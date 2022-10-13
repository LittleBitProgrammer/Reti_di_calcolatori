#include <strings.h>            /* Importata per utilizzare la funzione di azzeramento dei byte di un array "@bzero()" */
#include "sockets_utility.h"    /* Importata per utilizzare le funzioni:
                                        1. "@FullRead()"
                                        2. "@FullWrite()"
                                   E le costanti:
                                        1. CMD_BUFFER_LEN
                                */
#include "thread_utility.h"

void* vaccination_center_handler(void* args)
{
    /* ==========================
     * =       VARIABLES        =
     * ==========================
     * */
    int  connection_file_descriptor = *((int*)args);   /* File descriptor del socket che si occuperà di gestire nuove connessioni al server */
    char command_reader_buffer[CMD_BUFFER_LEN];        /* Buffer utile all'operazione di lettura del comando inviato dal client sul file descriptor del socket */
    char command_writer_buffer[CMD_BUFFER_LEN];        /* Buffer utile all'operazione di scrittura del comando da inviare sul file descriptor del socket */

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
    FullRead(connection_file_descriptor, &command_reader_buffer, CMD_BUFFER_LEN);

    /*
     * ==================================
     * =         CLOSE  THREAD          =
     * ==================================
     * */

    /*
     * Tale funzione ci permette di terminare il thread chiamante. Viene passato "@NULL" come argomento in quanto non si vuole
     * reperire l'informazione relativa al prossimo thread disponibile
     * */
    pthread_exit(NULL);
}