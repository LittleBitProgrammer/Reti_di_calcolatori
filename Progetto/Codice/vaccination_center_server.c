#include <stdio.h>
#include <netinet/in.h>           /* Importata per utilizzare la struttura sockaddr_in */
#include "lib/thread_utility.h"   /* Importata per utilizzare costanti e funzioni legate alla tecnologia thread */

int main(int argc, char **argv)
{
    /* ==========================
     * =       VARIABLES        =
     * ==========================
     * */
    int                listen_file_descriptor;                  /* File descriptor del socket in ascolto sul server */
    int                connection_file_descriptor;              /* File descriptor del socket che si occuperà di gestire nuove connessioni al server */
    int                is_address_reusable = 1;                 /* Valore intero che permette di configurare il server per il riutilizzo di un indirizzo
                                                                 * su cui è già stata effettuata la "@bind()" */
    struct sockaddr_in server_address;                          /* Struttura utile a rappresentare un Endpoint, in particolare l'indirizzo del server */
    struct sockaddr_in client_address;                          /* Struttura utile a rappresentare un Endpoint, in particolare l'indirizzo del client */
    socklen_t          client_size = sizeof(client_address);    /* Grandezza espressa in termini di byte dell'Endpoint client */
    int                i;                                       /* Variabile utilizzata da indice per i costrutti iterativi */
    pthread_t          threads_id[MAX_THREADS];                 /* Array contenente i descrittori dei threads utilizzati dal server */

    /* ==========================
     * =    SOCKET_CREATION     =
     * ==========================
     * */

    return 0;
}