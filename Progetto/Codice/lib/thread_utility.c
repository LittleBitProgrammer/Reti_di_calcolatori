#include <stdio.h>              /* Importata per utilizzare la funzione "@snprintf()" */
#include <stdlib.h>
#include <string.h>             /* Importata per utilizzare la funzione di azzeramento dei byte di un array "@bzero()" */
#include <time.h>               /* Importata per utilizzare la funzione "@time()" */
#include <unistd.h>             /* Importata per utilizzare la funzione "@close() */
#include <sys/syscall.h>
#include "sockets_utility.h"    /* Importata per utilizzare le funzioni:
                                        1. "@FullRead()"
                                        2. "@FullWrite()"
                                   E le costanti:
                                        1. CMD_BUFFER_LEN
                                */
#include "thread_utility.h"
#include "package_utility.h"
#include "file_utility.h"
#include "date_utility.h"
#include "green_pass_utility.h"

#define LOG TRUE
#include <unistd.h>
#include <sys/syscall.h>

#ifndef SYS_gettid
#error "SYS_gettid unavailable on this system"
#endif

#define gettid() ((pid_t)syscall(SYS_gettid))

bool is_main_thread(void)
{
    return getpid() == gettid();
}

/**
 * @brief
 *
 * @param args
 * */
void* vaccination_center_handler(void* args)
{
    /* Rende STDOUT non bufferizzato */
    (void)setvbuf(stdout, NULL, _IONBF, 0);
    /* ==========================
     * =       VARIABLES        =
     * ==========================
     * */
    int                connection_file_descriptor = ((Args*)args)->file_descriptor;   /* File descriptor del socket che si occuperà di gestire nuove connessioni al server */
    struct sockaddr_in client_address = *(((Args*)args)->endpoint);
    char               command_reader_buffer[CMD_BUFFER_LEN];        /* Buffer utile all'operazione di lettura del comando inviato descriptor del socket */
    char               command_writer_buffer[CMD_BUFFER_LEN];        /* Buffer utile all'operazione di scrittura del comando da inviare sul file descriptor del socket */
    time_t             server_daytime;                               /* Variabile utile a contenere un valore intero rappresentante il numero di secondi da 00:00, 1 gennaio 1970 */
    struct tm*         local_daytime;                                /* Struttura utile a memorizzare data e tempo locale suddivisi in campi */
    Vaccinated_package vaccinated_response_package;                  /*  */

    while(1)
    {
        /* =========================
         * =        ZEROING        =
         * =========================
         * */

        /*
         * Azzeriamo i byte che compongono l'array "@command_reader_buffer" e "@command_writer_buffer" per evitare di avere
         * valori raw all'interno di questi ultimi
         * */
        bzero(command_reader_buffer, CMD_BUFFER_LEN);
        bzero(command_writer_buffer, CMD_BUFFER_LEN);
        bzero(&vaccinated_response_package, sizeof(vaccinated_response_package));

        /*
         * ==================================
         * =           FULL READ            =
         * ==================================
         * */

        /* Eseguiamo un'operazione di lettura sul socket per leggere il comando inviato dal client */
        if(FullRead(connection_file_descriptor, &command_reader_buffer, CMD_BUFFER_LEN) > 0)
        {
            /*
             * ==================================
             * =         CLOSE  THREAD          =
             * ==================================
             * */

            /* Caso in cui il client si sia disconnesso */
            #ifdef LOG
            LogHostIPV4(&client_address, "Client disconnected:", NULL);
            #endif
            /* Chiusura del socket file descriptor connesso al client */
            close(connection_file_descriptor);
            /*
             * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
             * reperire l'informazione relativa al prossimo thread disponibile
             * */
            pthread_exit(NULL);
        }
        #ifdef LOG
        LogHostIPV4(&client_address, "Request received from", "Command request");
        #endif
        /*
         * ================================
         * =       COMMAND  COMPARE       =
         * ================================
         * */
        if(!strcmp(command_reader_buffer, "CMD_DTM"))
        {
            /*
             * =================================
             * =       DAYTIME  RESPONSE       =
             * =================================
             * */

            /* Sezione che gestisce il comando "@CMD_DTM" mandato dal client per servire la richiesta del daytime al client */

            /* Tale funzione ritorna un valore intero rappresentante il numero di secondi da 00:00, 1 gennaio 1970 */
            server_daytime = time(NULL);
            /* Tale funzione ci permette di convertire un tipo "@time_t" in una struttura utile a memorizzare data e tempo locale suddivisi in campi */
            if((local_daytime = localtime(&server_daytime)) == NULL)
            {
                fprintf(stderr, "local day time error\n");

                close(connection_file_descriptor);
                pthread_exit(NULL);
            }

            /* Ci avvaliamo della funzione "@FullWrite" per scrivere sul socket i bytes che compongono la struttura di tipo "@tm" */
            FullWrite(connection_file_descriptor, local_daytime, sizeof(*local_daytime));
            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
        }
        else if(!strcmp(command_reader_buffer, "CMD_SUB"))
        {
            /*
             * =================================
             * =     SUBSCRIPTION  RESPONSE    =
             * =================================
             * */

            int subscription_socket;
            struct sockaddr_in central_server_address;
            const char central_server_ip[] = "127.0.0.1";
            File_result is_card_code_written;

            /*
             *
             * */
            if(FullRead(connection_file_descriptor, &vaccinated_response_package, sizeof(vaccinated_response_package)) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&client_address, "Client disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&client_address, "Response received from", command_reader_buffer);
            #endif

            /* ==========================
             * =    SOCKET CREATION     =
             * ==========================
             * */

            subscription_socket = SocketIPV4();

            /*
             * ==================================
             * =        SERVER CREATION         =
             * ==================================
             * */

            /*
             * Inizializziamo i campi della struttura "@sockaddr_in" del server in modo tale da costruire un Endpoint identificabile sulla rete.
             * La struttura "@sockaddr_in" è composta dai seguenti campi:
             *      @sin_family:      Famiglia degli indirizzi utilizzati (AF_INET - AF_INET6 - ecc...)
             *      @sin_port:        Porta in Network order
             *      @sin_addr.s_addr: Indirizzo IP in Network order
             * */

            /*
             * Inizializziamo il campo famiglia della struttura "@sockaddr_in" del server con il valore "@AF_INET". In questo modo, specifichiamo
             * che il nostro server utilizzerà un indirizzo del tipo IPv4
             * */
            central_server_address.sin_family = AF_INET;

            /*
             *
             * */
            if(inet_pton(AF_INET, central_server_ip, &central_server_address.sin_addr) <= 0)
            {
                /*  */
                fprintf(stderr, "inet_pton() error for %s\n", central_server_ip);
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                close(subscription_socket);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }

            /*
             * Inizializziamo il campo porta della struttura "@sockaddr_in" del server attraverso il valore di ritorno della funzione
             * "@htons()" la quale accetterà come argomento un intero rappresentante la porta desiderata su cui il server deve rimanere
             * in ascolto. Le porte sono interi a 16 bit da 0 a 65535, raggruppate nel seguente modo:
             *      - da 0 a 1023, porte riservate ai processi root;
             *      - da 1024 a 49151, porte registrate;
             *      - da 49152 a 65535, porte effimere, per i client, ai quali non interessa scegliere una porta specifica.
             * Per il progetto si è deciso di utilizzare una porta registrata "6464"
             * */
            central_server_address.sin_port = htons(6464);

            /*
             * ==================================
             * =           CONNECTION           =
             * ==================================
             * */

            /* Eseguiamo una richiesta di Three way Handshake alla struttura "@sockaddr_in" del server precedentemente generata */
            ConnectIPV4(subscription_socket, &central_server_address);

            strcpy(command_writer_buffer, "CMD_CTR");

            FullWrite(subscription_socket, command_writer_buffer, CMD_BUFFER_LEN);
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
            #endif
            FullWrite(subscription_socket, vaccinated_response_package.card_code, sizeof(vaccinated_response_package.card_code));
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
            #endif

            if(FullRead(subscription_socket, &is_card_code_written, sizeof(File_result)) > 0)
            {
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Server disconnected from:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                close(subscription_socket);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Response received from", command_reader_buffer);
            #endif

            if(is_card_code_written.file_flags.open_file_flag)
            {
                close(subscription_socket);

                is_card_code_written.result_flag = !is_card_code_written.result_flag;
                FullWrite(connection_file_descriptor, &is_card_code_written, sizeof(File_result));
                #ifdef LOG
                LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
                #endif

                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            else if(!is_card_code_written.result_flag)
            {
                File_result is_subscribed;
                struct tm expiration_date = add_month_to_date(vaccinated_response_package.vaccination_date, EXPIRATION_DURATION_MONTH);
                Subscribe_package sub_client_vaccination = {.vaccinated_package.vaccination_date = vaccinated_response_package.vaccination_date,
                        .expiration_date = expiration_date};
                strcpy(sub_client_vaccination.vaccinated_package.card_code, vaccinated_response_package.card_code);

                strcpy(command_writer_buffer, "CMD_MEM");

                FullWrite(subscription_socket, command_writer_buffer, CMD_BUFFER_LEN);
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
                #endif
                FullWrite(subscription_socket, &sub_client_vaccination, sizeof(sub_client_vaccination));
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
                #endif

                FullRead(subscription_socket, &is_subscribed, sizeof(File_result));
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Response received from", command_reader_buffer);
                #endif
                close(subscription_socket);

                FullWrite(connection_file_descriptor, &is_subscribed, sizeof(File_result));
                #ifdef LOG
                LogHostIPV4(&client_address, "Request sent to", command_reader_buffer);
                #endif
            }
            else
            {
                close(subscription_socket);

                is_card_code_written.result_flag = !is_card_code_written.result_flag;
                FullWrite(connection_file_descriptor, &is_card_code_written, sizeof(File_result));
                #ifdef LOG
                LogHostIPV4(&client_address, "Request sent to", command_reader_buffer);
                #endif
            }
        }
    }
}

void* central_server_handler(void* args)
{
    /* Rende STDOUT non bufferizzato */
    (void)setvbuf(stdout, NULL, _IONBF, 0);

    int                connection_file_descriptor = ((Args*)args)->file_descriptor; /* File descriptor del socket che si occuperà di gestire nuove connessioni al server */
    struct sockaddr_in client_address = *(((Args*)args)->endpoint);
    char command_reader_buffer[CMD_BUFFER_LEN];
    char reader_buffer[21];
    File_result is_card_code_written;

    while(1)
    {
        /* =========================
         * =        ZEROING        =
         * =========================
         * */

        /*
         * Azzeriamo i byte che compongono l'array "@command_reader_buffer" e "@command_writer_buffer" per evitare di avere
         * valori raw all'interno di questi ultimi
         * */
        bzero(command_reader_buffer, CMD_BUFFER_LEN);
        bzero(reader_buffer, 21);

        /*
         *
         * */
        if(FullRead(connection_file_descriptor, &command_reader_buffer, CMD_BUFFER_LEN) > 0)
        {
            /*
             * ==================================
             * =         CLOSE  THREAD          =
             * ==================================
             * */

            /* Caso in cui il client si sia disconnesso */
            #ifdef LOG
            LogHostIPV4(&client_address, "Client disconnected:", NULL);
            #endif
            /* Chiusura del socket file descriptor connesso al client */
            close(connection_file_descriptor);
            /*
             * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
             * reperire l'informazione relativa al prossimo thread disponibile
             * */
            pthread_exit(NULL);
        }
        #ifdef LOG
        LogHostIPV4(&client_address, "Request received from", "Command request");
        #endif
        /*
         * ================================
         * =       COMMAND  COMPARE       =
         * ================================
         * */
        if(!strcmp(command_reader_buffer, "CMD_CTR"))
        {
            if(FullRead(connection_file_descriptor, &reader_buffer, 21) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&client_address, "Client disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&client_address, "Request received from", command_reader_buffer);
            #endif
            is_card_code_written = is_code_written_in_file(VACCINATED_FILE_NAME, reader_buffer);
            FullWrite(connection_file_descriptor, &is_card_code_written, sizeof(File_result));
            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
        }
        else if(!strcmp(command_reader_buffer, "CMD_MEM"))
        {
            Subscribe_package sub_client_vaccination;
            char file_writer_buffer[MAX_FILE_LINE_SIZE];
            char expiration_date_buffer[MAX_DATE_LEN];
            char last_update_date_buffer[MAX_DATE_LEN];
            File_result is_subscribed;


            if(FullRead(connection_file_descriptor, &sub_client_vaccination, sizeof(sub_client_vaccination)) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&client_address, "Client disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&client_address, "Request received from", command_reader_buffer);
            #endif

            strftime(expiration_date_buffer, MAX_DATE_LEN, "%d/%m/%Y", &(sub_client_vaccination.expiration_date));
            strftime(last_update_date_buffer, MAX_DATE_LEN, "%d/%m/%Y", &(sub_client_vaccination.vaccinated_package.vaccination_date));

            snprintf(file_writer_buffer, MAX_FILE_LINE_SIZE, "%s %s Vaccinazione %s", sub_client_vaccination.vaccinated_package.card_code,
                                                                                                       expiration_date_buffer,
                                                                                                       last_update_date_buffer);

            is_subscribed = subscribe_vaccinated_client(file_writer_buffer);
            FullWrite(connection_file_descriptor, &is_subscribed, sizeof(File_result));
            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
            if(!is_subscribed.result_flag)
            {
                fprintf(stderr, "Errore durante l'inserimento a file del vaccinato\n");
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            else
            {
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
        }
        else if(!strcmp(command_reader_buffer, "CMD_REV"))
        {
            Reviser_package reviser_package;

            if(FullRead(connection_file_descriptor, reader_buffer, 21) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&client_address, "Client disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&client_address, "Request received from", command_reader_buffer);
            #endif
            if(is_code_written_in_file(VACCINATED_FILE_NAME, reader_buffer).result_flag)
            {
                if(generate_reviser_response(&reviser_package, reader_buffer) != NULL)
                {
                    /* Chiusura del socket file descriptor connesso al client */
                    close(connection_file_descriptor);
                    /*
                     * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                     * reperire l'informazione relativa al prossimo thread disponibile
                     * */
                    pthread_exit(NULL);
                }
            }
            else
            {
                bzero(&reviser_package, sizeof(reviser_package));
                reviser_package.file_flags.read_file_flag = TRUE;
            }

            FullWrite(connection_file_descriptor, &reviser_package, sizeof(reviser_package));
            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
        }
        else if(!strcmp(command_reader_buffer, "CMD_LST"))
        {
            char* vaccinated_array_list;
            int size_vaccinated_list;

            if((size_vaccinated_list = read_lines(VACCINATED_FILE_NAME, &vaccinated_array_list)) == -1)
            {
                #ifdef LOG
                LogHostIPV4(&client_address, "Disconnected from client:", NULL);
                #endif

                free(vaccinated_array_list);
                close(connection_file_descriptor);
                pthread_exit(NULL);
            }

            FullWrite(connection_file_descriptor, &size_vaccinated_list, sizeof(int));
            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
            FullWrite(connection_file_descriptor, vaccinated_array_list, size_vaccinated_list * 21);
            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif

            free(vaccinated_array_list);
        }
        else if(!strcmp(command_reader_buffer, "CMD_MOD"))
        {
            Administrator_request_package update_information_package;
            Administrator_response_package administrator_response_package;

            if(FullRead(connection_file_descriptor, &update_information_package, sizeof(update_information_package)) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&client_address, "Client disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&client_address, "Request received from", command_reader_buffer);
            #endif

            if(!change_information_in_file(&update_information_package, &administrator_response_package, VACCINATED_FILE_NAME))
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }

            FullWrite(connection_file_descriptor, &administrator_response_package, sizeof(administrator_response_package));
            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
        }
    }
}

void* assistant_server_handler(void* args)
{
    /* Rende STDOUT non bufferizzato */
    (void)setvbuf(stdout, NULL, _IONBF, 0);
    /* ==========================
     * =       Variables        =
     * ==========================
     */
    int                connection_file_descriptor = ((Args*)args)->file_descriptor;   /* File descriptor del socket che si occuperà di gestire nuove connessioni al server */
    struct sockaddr_in client_address = *(((Args*)args)->endpoint);
    char command_reader_buffer[CMD_BUFFER_LEN];
    char code_reader_buffer[21];

    while(1)
    {
        /* ==========================
         * =    ZEROING  ARRAYS     =
         * ==========================
         * */

        bzero(command_reader_buffer, CMD_BUFFER_LEN);
        bzero(code_reader_buffer, 21);

        /*
         * ==================================
         * =           FULL READ            =
         * ==================================
         * */
        if (FullRead(connection_file_descriptor, command_reader_buffer, CMD_BUFFER_LEN) > 0) {
            /*
             * ==================================
             * =         CLOSE  THREAD          =
             * ==================================
             * */

            /* Caso in cui il client si sia disconnesso */
            #ifdef LOG
            LogHostIPV4(&client_address, "Client disconnected:", NULL);
            #endif
            /* Chiusura del socket file descriptor connesso al client */
            close(connection_file_descriptor);
            /*
             * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
             * reperire l'informazione relativa al prossimo thread disponibile
             * */
            pthread_exit(NULL);
        }
        #ifdef LOG
        LogHostIPV4(&client_address, "Request received from", "Command request");
        #endif

        /*
         * ================================
         * =       COMMAND  COMPARE       =
         * ================================
         * */
        if (!strcmp(command_reader_buffer, "CMD_REV")) {
            if (FullRead(connection_file_descriptor, code_reader_buffer, 21) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&client_address, "Client disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&client_address, "Request received from", command_reader_buffer);
            #endif

            /*
             * =========================
             * =       VARIABLES       =
             * =========================
             * */
            int reviser_socket;
            struct sockaddr_in central_server_address;
            const char central_server_ip[] = "127.0.0.1";
            Reviser_package reviser_package;

            /* ==========================
             * =    SOCKET CREATION     =
             * ==========================
             * */

            reviser_socket = SocketIPV4();

            /*
             * ==================================
             * =        SERVER CREATION         =
             * ==================================
             * */

            /*
             * Inizializziamo i campi della struttura "@sockaddr_in" del server in modo tale da costruire un Endpoint identificabile sulla rete.
             * La struttura "@sockaddr_in" è composta dai seguenti campi:
             *      @sin_family:      Famiglia degli indirizzi utilizzati (AF_INET - AF_INET6 - ecc...)
             *      @sin_port:        Porta in Network order
             *      @sin_addr.s_addr: Indirizzo IP in Network order
             * */

            /*
             * Inizializziamo il campo famiglia della struttura "@sockaddr_in" del server con il valore "@AF_INET". In questo modo, specifichiamo
             * che il nostro server utilizzerà un indirizzo del tipo IPv4
             * */
            central_server_address.sin_family = AF_INET;

            /*
             *
             * */
            if(inet_pton(AF_INET, central_server_ip, &central_server_address.sin_addr) <= 0)
            {
                /*  */
                fprintf(stderr, "inet_pton() error for %s\n", central_server_ip);
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                close(reviser_socket);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }

            /*
             * Inizializziamo il campo porta della struttura "@sockaddr_in" del server attraverso il valore di ritorno della funzione
             * "@htons()" la quale accetterà come argomento un intero rappresentante la porta desiderata su cui il server deve rimanere
             * in ascolto. Le porte sono interi a 16 bit da 0 a 65535, raggruppate nel seguente modo:
             *      - da 0 a 1023, porte riservate ai processi root;
             *      - da 1024 a 49151, porte registrate;
             *      - da 49152 a 65535, porte effimere, per i client, ai quali non interessa scegliere una porta specifica.
             * Per il progetto si è deciso di utilizzare una porta registrata "6464"
             * */
            central_server_address.sin_port = htons(6464);

            /*
             * ==================================
             * =           CONNECTION           =
             * ==================================
             * */

            /* Eseguiamo una richiesta di Three way Handshake alla struttura "@sockaddr_in" del server precedentemente generata */
            ConnectIPV4(reviser_socket, &central_server_address);
            FullWrite(reviser_socket, command_reader_buffer, CMD_BUFFER_LEN);

            #ifdef LOG
            LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
            #endif

            FullWrite(reviser_socket, code_reader_buffer, 21);

            #ifdef LOG
            LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
            #endif

            if(FullRead(reviser_socket, &reviser_package, sizeof(reviser_package)) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Server disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(reviser_socket);
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Response received from", command_reader_buffer);
            #endif

            close(reviser_socket);
            FullWrite(connection_file_descriptor, &reviser_package, sizeof(reviser_package));

            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
        }
        else if(!strcmp(command_reader_buffer, "CMD_LST"))
        {
            /*
             * =========================
             * =       VARIABLES       =
             * =========================
             * */
            int administrator_socket;
            struct sockaddr_in central_server_address;
            const char central_server_ip[] = "127.0.0.1";
            int size_codes_list;
            char* codes_list;


            /* ==========================
             * =    SOCKET CREATION     =
             * ==========================
             * */

            administrator_socket = SocketIPV4();

            /*
             * ==================================
             * =        SERVER CREATION         =
             * ==================================
             * */

            /*
             * Inizializziamo i campi della struttura "@sockaddr_in" del server in modo tale da costruire un Endpoint identificabile sulla rete.
             * La struttura "@sockaddr_in" è composta dai seguenti campi:
             *      @sin_family:      Famiglia degli indirizzi utilizzati (AF_INET - AF_INET6 - ecc...)
             *      @sin_port:        Porta in Network order
             *      @sin_addr.s_addr: Indirizzo IP in Network order
             * */

            /*
             * Inizializziamo il campo famiglia della struttura "@sockaddr_in" del server con il valore "@AF_INET". In questo modo, specifichiamo
             * che il nostro server utilizzerà un indirizzo del tipo IPv4
             * */
            central_server_address.sin_family = AF_INET;

            /*
             *
             * */
            if(inet_pton(AF_INET, central_server_ip, &central_server_address.sin_addr) <= 0)
            {
                /*  */
                fprintf(stderr, "inet_pton() error for %s\n", central_server_ip);
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                close(administrator_socket);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }

            /*
             * Inizializziamo il campo porta della struttura "@sockaddr_in" del server attraverso il valore di ritorno della funzione
             * "@htons()" la quale accetterà come argomento un intero rappresentante la porta desiderata su cui il server deve rimanere
             * in ascolto. Le porte sono interi a 16 bit da 0 a 65535, raggruppate nel seguente modo:
             *      - da 0 a 1023, porte riservate ai processi root;
             *      - da 1024 a 49151, porte registrate;
             *      - da 49152 a 65535, porte effimere, per i client, ai quali non interessa scegliere una porta specifica.
             * Per il progetto si è deciso di utilizzare una porta registrata "6464"
             * */
            central_server_address.sin_port = htons(6464);

            /*
             * ==================================
             * =           CONNECTION           =
             * ==================================
             * */

            /* Eseguiamo una richiesta di Three way Handshake alla struttura "@sockaddr_in" del server precedentemente generata */
            ConnectIPV4(administrator_socket, &central_server_address);

            FullWrite(administrator_socket, command_reader_buffer, CMD_BUFFER_LEN);
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
            #endif

            if(FullRead(administrator_socket, &size_codes_list, sizeof(int)) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Server disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(administrator_socket);
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Response received from", command_reader_buffer);
            #endif

            if((codes_list = (char*)malloc(size_codes_list * 21)) == NULL)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */
                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Server disconnected:", NULL);
                #endif

                free(codes_list);
                /* Chiusura del socket file descriptor connesso al client */
                close(administrator_socket);
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }

            if(FullRead(administrator_socket, codes_list, size_codes_list * 21) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */
                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Server disconnected:", NULL);
                #endif

                free(codes_list);
                /* Chiusura del socket file descriptor connesso al client */
                close(administrator_socket);
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Response received from", command_reader_buffer);
            #endif
            
            close(administrator_socket);
            FullWrite(connection_file_descriptor, &size_codes_list, sizeof(int));

            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif

            FullWrite(connection_file_descriptor, codes_list, size_codes_list * 21);

            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif

            free(codes_list);
        }
        else if(!strcmp(command_reader_buffer, "CMD_MOD"))
        {
            /*
             * =========================
             * =       VARIABLES       =
             * =========================
             * */
            int administrator_socket;
            struct sockaddr_in central_server_address;
            const char central_server_ip[] = "127.0.0.1";
            Administrator_request_package update_information_package;
            Administrator_response_package response_from_server;

            if(FullRead(connection_file_descriptor, &update_information_package, sizeof(update_information_package)) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&client_address, "Client disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }

            #ifdef LOG
            LogHostIPV4(&client_address, "Request received from", command_reader_buffer);
            #endif


            /* ==========================
             * =    SOCKET CREATION     =
             * ==========================
             * */

            administrator_socket = SocketIPV4();

            /*
             * ==================================
             * =        SERVER CREATION         =
             * ==================================
             * */

            /*
             * Inizializziamo i campi della struttura "@sockaddr_in" del server in modo tale da costruire un Endpoint identificabile sulla rete.
             * La struttura "@sockaddr_in" è composta dai seguenti campi:
             *      @sin_family:      Famiglia degli indirizzi utilizzati (AF_INET - AF_INET6 - ecc...)
             *      @sin_port:        Porta in Network order
             *      @sin_addr.s_addr: Indirizzo IP in Network order
             * */

            /*
             * Inizializziamo il campo famiglia della struttura "@sockaddr_in" del server con il valore "@AF_INET". In questo modo, specifichiamo
             * che il nostro server utilizzerà un indirizzo del tipo IPv4
             * */
            central_server_address.sin_family = AF_INET;

            /*
             *
             * */
            if(inet_pton(AF_INET, central_server_ip, &central_server_address.sin_addr) <= 0)
            {
                /*  */
                fprintf(stderr, "inet_pton() error for %s\n", central_server_ip);
                /* Chiusura del socket file descriptor connesso al client */
                close(connection_file_descriptor);
                close(administrator_socket);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }

            /*
             * Inizializziamo il campo porta della struttura "@sockaddr_in" del server attraverso il valore di ritorno della funzione
             * "@htons()" la quale accetterà come argomento un intero rappresentante la porta desiderata su cui il server deve rimanere
             * in ascolto. Le porte sono interi a 16 bit da 0 a 65535, raggruppate nel seguente modo:
             *      - da 0 a 1023, porte riservate ai processi root;
             *      - da 1024 a 49151, porte registrate;
             *      - da 49152 a 65535, porte effimere, per i client, ai quali non interessa scegliere una porta specifica.
             * Per il progetto si è deciso di utilizzare una porta registrata "6464"
             * */
            central_server_address.sin_port = htons(6464);

            /*
             * ==================================
             * =           CONNECTION           =
             * ==================================
             * */

            /* Eseguiamo una richiesta di Three way Handshake alla struttura "@sockaddr_in" del server precedentemente generata */
            ConnectIPV4(administrator_socket, &central_server_address);

            FullWrite(administrator_socket, command_reader_buffer, CMD_BUFFER_LEN);

            #ifdef LOG
            LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
            #endif

            FullWrite(administrator_socket, &update_information_package, sizeof(update_information_package));

            #ifdef LOG
            LogHostIPV4(&central_server_address, "Request sent to", command_reader_buffer);
            #endif


            if(FullRead(administrator_socket, &response_from_server, sizeof(response_from_server)) > 0)
            {
                /*
                 * ==================================
                 * =         CLOSE  THREAD          =
                 * ==================================
                 * */

                /* Caso in cui il client si sia disconnesso */
                #ifdef LOG
                LogHostIPV4(&central_server_address, "Server disconnected:", NULL);
                #endif
                /* Chiusura del socket file descriptor connesso al client */
                close(administrator_socket);
                close(connection_file_descriptor);
                /*
                 * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
                 * reperire l'informazione relativa al prossimo thread disponibile
                 * */
                pthread_exit(NULL);
            }
            #ifdef LOG
            LogHostIPV4(&central_server_address, "Response received from", command_reader_buffer);
            #endif

            close(administrator_socket);
            FullWrite(connection_file_descriptor, &response_from_server, sizeof(response_from_server));

            #ifdef LOG
            LogHostIPV4(&client_address, "Response sent to", command_reader_buffer);
            #endif
        }
    }
}