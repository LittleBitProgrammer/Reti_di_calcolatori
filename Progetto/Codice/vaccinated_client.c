#include <stdio.h>                  /* Importata per utilizzare la funzione "@perror()" */
#include <stdlib.h>                 /* Importata per utilizzare la funzione "@exit()" */
#include <netdb.h>                  /* Importata per utilizzare la funzione "@gethostbyname()" */
#include <string.h>                 /* Importata per utilizzare la funzione "@bzero()" */
#include <unistd.h>                 /* Importata per utilizzare la costante "@STDIN_FILENO" */
#include "lib/sockets_utility.h"    /* Importata per utilizzare funzioni wrapper per la gestione dei socket */
#include "lib/menu_utility.h"       /* Importata per utilizzare la funzione "@print_vaccinated_menu()" */
#include "lib/package_utility.h"    /*  */

#define CL 21

int main(int argc, char **argv)
{
    /* ==========================
     * =       Variables        =
     * ==========================
     */
    int                client_file_descriptor;                                       /* File descriptor associato al socket del client */
    struct sockaddr_in server_address;                                               /* Struttura utile a rappresentare un Endpoint, in
                                                                                        particolare l'indirizzo del server */
    struct hostent*    server_dns;                                                   /* Struttura contenente varie informazioni sull'host server,
                                                                                        utile ad effettuare un'operazione DNS (Risoluzione diretta) */
    char               command_writer_buffer[CMD_BUFFER_LEN];                        /* Buffer utile all'operazione di scrittura del
                                                                                        comando da inviare sul file descriptor del socket */
    struct tm*         server_daytime = (struct tm*)malloc(sizeof(struct tm));  /* Struttura utile a memorizzare data e tempo locale suddivisi in campi */
    struct tm*         client_daytime = (struct tm*)malloc(sizeof(struct tm));  /* Struttura utile a memorizzare data e tempo di vaccinazione
                                                                                        da parte del client */
    char*              verification_code = (char *)malloc(CL * sizeof(char));   /*  */
    Vaccinated_package vaccinated_request_package;                                   /*  */
    Error_handling     is_green_pass_obtained;                                       /*  */

    /*
     * ==========================
     * =       Arguments        =
     * ==========================
     * */

    /*
     * Attraverso il seguente costrutto di controllo si va a controllare che sia stato inserito un numero di argomenti pari ad 1,
     * in quanto anche il comando digitato su linea di comando viene contato come argomento
     * */
    if(argc != 2)
    {
        fprintf(stderr, "Client usage: %s <IP ADDRESS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* ==========================
     * =    ZEROING  ARRAYS     =
     * ==========================
     * */

    /*
     * Azzeriamo i byte che compongono l'array "@command_writer_buffer", "@client_daytime" e "@server_daytime" per evitare di avere valori raw
     * all'interno di quest'ultimo
     * */
    bzero(command_writer_buffer, CMD_BUFFER_LEN);
    bzero(client_daytime, sizeof(*client_daytime));
    bzero(server_daytime, sizeof(*server_daytime));
    bzero(verification_code, CL);
    bzero(&vaccinated_request_package, sizeof(vaccinated_request_package));

    /*
     * ==========================
     * =          DNS           =
     * ==========================
     * */

    /*
     * La funzione "@gethostbyname()" ritorna una struttura di tipo "@hostent" in base al nome o all'indirizzo IPv4 dell'host fornito in input.
     * Inoltre, se la funzione non è andata a buon fine, allora verrà ritornato un puntatore a "@NULL" e assegnato il numero dell'errore alla
     * variabile "@h_errno" intercettata successivamente dal "@herror()"
     * */
    if((server_dns = gethostbyname(argv[1])) == NULL)
    {
        /* La seguente funzione produce un messaggio sullo standard error (file descriptor: 2) che descrive la natura dell'errore */
        herror("DNS error: ");
        exit(EXIT_FAILURE);
    }

    /*
     * ==================================
     * =        SOCKET CREATION         =
     * ==================================
     * */

    /*
    * Inizializziamo il valore del file descriptor sfruttando la funzione definita nella libreria "@sockets_utilities.h".
     * In questo modo, associamo il file descriptor ad una socket
    * */
    client_file_descriptor = SocketIPV4();

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
     * Inizializziamo il campo famiglia della struttura "@sockaddr_in" del server con il valore "@server_dns->h_addrtype",
     * reperito attraverso un azione di risoluzione diretta del DNS. In questo modo, specifichiamo che il nostro server
     * utilizzerà un indirizzo del tipo IPv4
     * */
     server_address.sin_family = server_dns->h_addrtype;

     /*
      * Inizializziamo il campo della struttura "@sockaddr_in" del server attraverso il valore "@server_dns->h_addr_list[0]"
      * Sfruttiamo un cast a puntatore della struttura "@in_addr" in modo da leggere i singoli byte che compongono "@h_addr_list[0]"
      * come un intero definito nella struttura "@sin_addr.s_addr". In questo modo si memorizza l'indirizzo IP ottenuto
      * attraverso un'operazione diretta DNS in formato Network order
      * */
     server_address.sin_addr = *((struct in_addr *)server_dns->h_addr_list[0]);

    /*
     * Inizializziamo il campo porta della struttura "@sockaddr_in" del server attraverso il valore di ritorno della funzione
     * "@htons()" la quale accetterà come argomento un intero rappresentante la porta desiderata.
     * Le porte sono interi a 16 bit da 0 a 65535, raggruppate nel seguente modo:
     *      - da 0 a 1023, porte riservate ai processi root;
     *      - da 1024 a 49151, porte registrate;
     *      - da 49152 a 65535, porte effimere, per i client, ai quali non interessa scegliere una porta specifica.
     * Per il progetto si è deciso di utilizzare una porta registrata "6463"
     * */
     server_address.sin_port = htons(6463);

    /*
     * ==================================
     * =           CONNECTION           =
     * ==================================
     * */

    /* Eseguiamo una richiesta di Three way Handshake alla struttura "@sockaddr_in" del server precedentemente generata */
    ConnectIPV4(client_file_descriptor, &server_address);

    /*
     * ==================================
     * =        DAYTIME  REQUEST        =
     * ==================================
     * */

    /*
     * Di seguito, andremo ad eseguire una richiesta Daytime al server centro vaccinale, in modo tale da reperire la data
     * corrente del server, la quale ci servirà per verificare che la data inserita dall'utente non sia superiore alla data
     * odierna
     * */

    /* Copiamo la stringa "CMD_DTM" all'interno dell'array di caratteri "@command_writer_buffer" */
    strcpy(command_writer_buffer, "CMD_DTM");

    /* Effettuiamo una richiesta daytime al server con le informazioni contenute nel "@command_writer_buffer" */
    FullWrite(client_file_descriptor, command_writer_buffer, CMD_BUFFER_LEN);

    /*
     * ==================================
     * =       DAYTIME   RESPONSE       =
     * ==================================
     * */

    /* Eseguiamo una "@FullRead()" per ottenere una response Daytime dal server connesso */
    if(FullRead(client_file_descriptor, server_daytime, sizeof(*server_daytime)) < 0)
    {
        /* Caso in cui il server si sia disconnesso */
        fprintf(stderr, "Server disconnesso\n");
        /* Liberiamo la memoria precedentemente allocata dinamicamente nella memoria heap tramite una "@malloc" */
        free(server_daytime);
        free(client_daytime);
        free(verification_code);
        /* Chiusura del socket file descriptor connesso al server */
        close(client_file_descriptor);
        /* Terminiamo con successo il processo client */
        exit(EXIT_FAILURE);
    }

    /*
     * ==================================
     * =     SUBSCRIPTION  REQUEST      =
     * ==================================
     * */

    /*
     *
     * */
    if(!run_vaccinated_menu(client_daytime, server_daytime, verification_code))
    {
        /* Liberiamo la memoria precedentemente allocata dinamicamente nella memoria heap tramite una "@malloc" */
        free(server_daytime);
        free(client_daytime);
        free(verification_code);
        /* Chiusura del socket file descriptor connesso al server */
        close(client_file_descriptor);
        /* Terminiamo con successo il processo client */
        exit(EXIT_FAILURE);
    }

    /*  */
    vaccinated_request_package.vaccination_date = *client_daytime;
    strcpy(vaccinated_request_package.card_code, verification_code);

    /* Copiamo la stringa "CMD_SUB" all'interno dell'array di caratteri "@command_writer_buffer" */
    strcpy(command_writer_buffer, "CMD_SUB");

    /* Effettuiamo una richiesta daytime al server con le informazioni contenute nel "@command_writer_buffer" */
    FullWrite(client_file_descriptor, command_writer_buffer, CMD_BUFFER_LEN);

    /*  */
    FullWrite(client_file_descriptor, &vaccinated_request_package, sizeof(vaccinated_request_package));

    /*  */
    if(FullRead(client_file_descriptor, &is_green_pass_obtained, sizeof(Error_handling)) > 0)
    {
        /* Liberiamo la memoria precedentemente allocata dinamicamente nella memoria heap tramite una "@malloc" */
        free(server_daytime);
        free(client_daytime);
        free(verification_code);
        /* Chiusura del socket file descriptor connesso al server */
        close(client_file_descriptor);
        /* Terminiamo con successo il processo client */
        exit(EXIT_FAILURE);
    }

    if(is_green_pass_obtained.result_flag)
    {
        printf("Caricato con successo\n");
    }
    else if(is_green_pass_obtained.write_file_flag || is_green_pass_obtained.read_file_flag || is_green_pass_obtained.open_file_flag)
    {
        fprintf(stderr,"Anomalia durante l'operazione del server\n");
    }
    else
    {
        fprintf(stderr,"Errore nel caricamento\n");
    }

    /* Liberiamo la memoria precedentemente allocata dinamicamente nella memoria heap tramite una "@malloc" */
    free(server_daytime);
    free(client_daytime);
    free(verification_code);
    /* Chiusura del socket file descriptor connesso al server */
    close(client_file_descriptor);
    /* Terminiamo con successo il processo client */
    exit(EXIT_SUCCESS);
}
