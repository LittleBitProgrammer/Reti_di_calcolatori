#include <stdio.h>              /* Importata per utilizzare la funzione "@snprintf()" */
#include <string.h>             /* Importata per utilizzare la funzione di azzeramento dei byte di un array "@bzero()" */
#include <time.h>               /* Importata per utilizzare la funzione "@time()" */
#include <unistd.h>             /* Importata per utilizzare la funzione "@close() */
#include "sockets_utility.h"    /* Importata per utilizzare le funzioni:
                                        1. "@FullRead()"
                                        2. "@FullWrite()"
                                   E le costanti:
                                        1. CMD_BUFFER_LEN
                                */
#include "thread_utility.h"

/**
 * @brief
 *
 * @param args
 * */
void* vaccination_center_handler(void* args)
{
    /* ==========================
     * =       VARIABLES        =
     * ==========================
     * */
    int        connection_file_descriptor = *((int*)args);   /* File descriptor del socket che si occuperà di gestire nuove connessioni al server */
    char       command_reader_buffer[CMD_BUFFER_LEN];        /* Buffer utile all'operazione di lettura del comando inviato descriptor del socket */
    char       command_writer_buffer[CMD_BUFFER_LEN];        /* Buffer utile all'operazione di scrittura del comando da inviare sul file descriptor del socket */
    time_t     server_daytime;                               /* Variabile utile a contenere un valore intero rappresentante il numero di secondi da 00:00, 1 gennaio 1970 */
    struct tm* local_daytime;                                /* Struttura utile a memorizzare data e tempo locale suddivisi in campi */

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
            fprintf(stderr, "Client disconnesso\n");
            /* Chiusura del socket file descriptor connesso al client */
            close(connection_file_descriptor);
            /*
             * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
             * reperire l'informazione relativa al prossimo thread disponibile
             * */
            pthread_exit(NULL);
        }

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
            }

            /* Ci avvaliamo della funzione "@FullWrite" per scrivere sul socket i bytes che compongono la struttura di tipo "@tm" */
            FullWrite(connection_file_descriptor, local_daytime, sizeof(*local_daytime));
        }
        else if(!strcmp(command_reader_buffer, "CMD_SUB"))
        {
            /*
             * =================================
             * =     SUBSCRIPTION  RESPONSE    =
             * =================================
             * */

            /*
             *
             * */

        }
    }
}