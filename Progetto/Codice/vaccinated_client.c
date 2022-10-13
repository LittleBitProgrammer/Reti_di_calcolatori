#include <stdio.h>                  /* Importata per utilizzare la funzione "@perror()" */
#include <stdlib.h>                 /* Importata per utilizzare la funzione "@exit()" */
#include <netdb.h>                  /* Importata per utilizzare la funzione "@gethostbyname()" */
#include <string.h>                 /* Importata per utilizzare la funzione "@bzero()" */
#include <unistd.h>                 /* Importata per utilizzare la costante "@STDIN_FILENO" */
#include <time.h>                   /* Importata per utilizzare la struttura "@struct tm" */
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
    struct tm*         server_daytime;                          /* Struttura utile a memorizzare data e tempo locale suddivisi in campi */
    char stringa[100];

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

    /* Azzeriamo i byte che compongono l'array "@command_writer_buffer" per evitare di avere valori raw all'interno di quest'ultimo */
    bzero(command_writer_buffer, CMD_BUFFER_LEN);

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
     * ============================
     * =          SELECT          =
     * ============================
     * */

    /* Configuriamo il tempo massimo che una System call può attendere per individuare un descrittore pronto */
    timeout.tv_sec = SECONDS_TO_WAIT;
    timeout.tv_usec = MICROSECONDS_TO_WAIT;

    /*  */
    FD_ZERO(&monitor_reader_socket);
    /*  */
    FD_SET(STDIN_FILENO, &monitor_reader_socket);
    FD_SET(client_file_descriptor, &monitor_reader_socket);

    /*  */
    max_sockets_to_monitor = (client_file_descriptor > STDIN_FILENO) ? client_file_descriptor + 1 : STDIN_FILENO + 1;

    /*
     *
     * */
    if((ready_sockets = select(max_sockets_to_monitor, &monitor_reader_socket, NULL, NULL, &timeout)) < 0)
    {
        fprintf(stderr, "Select error\n");
        close(client_file_descriptor);
        exit(EXIT_FAILURE);
    }
    else if(ready_sockets == 0)
    {
        fprintf(stderr, "Session expired\n");
        close(client_file_descriptor);
        exit(EXIT_FAILURE);
    }
    else
    {
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
        if(FD_ISSET(STDIN_FILENO, &monitor_reader_socket))
        {
            scanf("%s", stringa);
            fprintf(stderr, "%s\n", stringa);
        }

        if(FD_ISSET(client_file_descriptor, &monitor_reader_socket))
        {
            if(FullRead(client_file_descriptor, server_daytime, sizeof(*server_daytime)) < 0)
            {
                /* Caso in cui il server si sia disconnesso */
                fprintf(stderr, "Server disconnesso\n");
                close(client_file_descriptor);
                exit(EXIT_FAILURE);
            }

        }

        exit(EXIT_SUCCESS);
    }
}
















