#include <stdio.h>                  /* Importata per utilizzare la funzione "@perror()" */
#include <stdlib.h>                 /* Importata per utilizzare la funzione "@exit()" */
#include <netdb.h>                  /* Importata per utilizzare la funzione "@gethostbyname()" */
#include "lib/sockets_utility.h"    /* Importata per utilizzare funzioni wrapper per la gestione dei socket */

int main(int argc, char **argv)
{
    /* ==========================
     * =       Variables        =
     * ==========================
     */
    int                client_file_descriptor;                  /* File descriptor associato al socket del client */
    struct sockaddr_in server_address;                          /* Struttura utile a rappresentare un Endpoint, in particolare l'indirizzo del server */
    struct hostent*    server_dns;                              /* Struttura contenente varie informazioni sull'host server, utile ad effettuare un'operazione
                                                                   DNS (Risoluzione diretta) */
    char               command_writer_buffer[CMD_BUFFER_LEN];   /* Buffer utile all'operazione di scrittura del comando da inviare sul file descriptor del socket */
    fd_set             monitor_reader_socket;                   /* Array, corrispondente all'insieme dei descrittori pronti per la lettura, dichiarato per
                                                                   implementare un I/O Multiplex sul client */
    int                max_sockets_to_monitor;                  /* Indice utile a limitare il numero di descrittori da controllare */
    size_t             ready_sockets;                           /* Numero di sockets pronti eseguire azioni di I/O */
    struct timeval     timeout;                                 /* Struttura utile a rappresentare il tempo massimo che una System call può attendere per individuare
                                                                   un descrittore pronto */

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

    

    return 0;
}
















