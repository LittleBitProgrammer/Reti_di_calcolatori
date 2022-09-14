/**
 * @file server.c
 * @author Roberto Vecchio
 * @brief Tale programma ha lo scopo di creare una struttura di tipo server. La sintassi utilizzata per lanciare questo programma è la seguente:
 *        ./nome_programma
 *
 *        Passi creazione server:
 *
 *        1. Controllo parametri in input della funzione main
 *        2. Creazione socket, attraverso la funzione socket() della libreria sys/socket.h
 *        3. Costruzione endpoint (server)
 * @version 0.1
 * @date 2022-09-14
 *
 * @copyright Copyright (c) 2022
 *
 */

/* Import librerie */
#include <stdio.h>      /* descrizione */
#include <stdlib.h>     /* descrizione */
#include <string.h>     /* descrizione */
#include <unistd.h>     /* descrizione */
#include <sys/socket.h> /* descrizione */
#include <arpa/inet.h>  /* descrizione */
#include <time.h>       /* descrizione */

/* main */
int main(int argc, char **argv)
{
    /*
    ======================================
           Dichiarazione Variabili
    ======================================
    */

    /* Socket descriptor */
    int listen_file_descriptor;     /* File descriptor del socket che si occuperà di rimanere in ascolto per le richieste di connessioni */
    int connection_file_descriptor; /* File descriptor del socket che si occuperà di gestire nuove connessioni */

    /* Endpoint server */
    struct sockaddr_in server_address; /* Struttura dati jutilizzata per rappresentare l'indirizzo del server */

    /* Buffer di scrittura */
    char write_buffer[4096]; /* Buffer utilizzato durante la scrittura sul file descriptor della socket di connessione */

    /* Timestamp attuale */
    time_t ticks; /* Secondi passati dal 1 gennaio 1970 */

    /*
     =========================================
             Creazione della socket
     =========================================
     */

    /*
     La funzione socket ritorna in output un descrittore di file, attraverso un id numerico maggiore di 0, univoco. Se il valore ritornato dalla funzione è minore di 0
     allora, ciò starà ad indicare che la creazione del socket non è andata a buon fine, pertanto andrà opportunamente ritornato un errore.

     La funzione socket accetterà in input:

     1. La famiglia degli indirizzi (ad es. IPV4 o IPV6)
     2. Protocollo di livello trasporto
     3. Sottotipo del protocollo di livello di trasporto (mai implementato, pertanto andrà sempre passato in input 0)
     */
    if ((listen_file_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        /* Stampiamo un errore etichettato dalla label Socket */
        perror("Socket");
        exit(1);
    }

    /*
    ============================================================
                 Costruzione endpoint (server)
    ============================================================
    */

    /*
    Per costruire un endpoint (server) abbiamo bisogno in particolare di due informazioni:

    ! - Indirizzo IP
    ? - Porta del processo server in esecuzione

    La struttura sockaddr_in permette di rappresentare univocamente, in C, un endpoint. Pertanto andremo opportunamente ad inizializzare tale struttura, per identificare
    univocamente il server, il cui indirizzo IP è stato passato in input.

    La struttura sockaddr_in è costituita dai seguenti campi:

    ! - sa_family_t sin_family     (Famiglia degli indirizzi ad es. IPV4 o IPV6)
    * - in_port_t sin_port		  (Port number a 16 bit)
    ? - struct in_addr sin_addr    (Internet address a 32 bit)

    Dove la struttura in_addr è definita dal seguente campo:

    ! - u_int32_t s_addr  (intero a 32 bit)

    Per l'inizializzazione della struttura viene utilizzata la seguente logica:

    ! - Il campo sin_family può essere inizializzato normalmente attraverso i valori conosciuti come ad esempio: AF_INET.
    * - Il campo sin_port deve essere inizializzato attraverso la funzione htons(int <numero_porta>) in quanto la porta deve essere codificata in big-endian
    *   secondo il network order
    ? - Il campo sin_addr.s_addr deve essere inzializzato attraverso la funzione htonl(uint32 <numero_indirizzo>) in quanto la porta deve essere codificata in
    ?   big-endian secondo il network order. Inoltre, verrà utilizzata una macro INADDR_ANY, che ci servirà a mettere in ascolto il socket su qualsiasi indirizzo IP
    ?   delle diverse interfacce di rete di cui il servver stesso è dotato
    */

    server_address.sin_family = AF_INET;                /* Famiglia degli indirizzi (IPV4)*/
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); /* Indirizzo IPV4 */
    server_address.sin_port = htons(13);                /* Porta del processo server */

    /*
      ============================================================
              Collegamento della socket con l'endpoint
      ============================================================
    */

    /*
    Il collegamento tra la socket e l'endpoint creato avvviene attravverso la funzione bind, che accetterà in input:

    ! 1. Il socket descriptor a cui vogliamo collegare l'endpoint (int <nome_file_descriptor)
    * 2. L'endpoint che vogliamo collegare al file descriptor. Però, tale dato dovrà essere castato a un tipo puntatore a struttura generica sockaddr, in quanto la funzione
    *    bind deve poter lavorare su endpoint generici
    ? 3. Grandezza in byte dell'endpoint passato in input. In questo modo la funzione riesce a interpretare su che tipo di endpoint sta lavorando
    */

    if (bind(listen_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        /* Stampiamo un errore etichettato dalla label Bind */
        perror("Bind");
        exit(1);
    }

    /*
      ============================================================
                  Configuriamo il socket in ascolto
      ============================================================
    */

    /*
    Possiamo configurare il socket listen_file_descriptor in ascolto, attraverso la funzione listen. Quest'ultima accetterà in input:

    ! 1. Il socket descriptor che deve rimanere in ascolto;
    * 2. Grandezza della coda delle richieste, ovvero il numero massimo di richieste che possono essere messe in coda ed attendere di essere servite
    */
    if (listen(listen_file_descriptor, 1024) < 0)
    {
        /* Stampiamo un errore etichettato dalla label Listen */
        perror("Listen");
        exit(1);
    }

    /* Eseguiamo un ciclo for infinito */
    for (;;)
    {
        /*
         ============================================================
                   Accettazione di una nuova connessione
         ============================================================
       */

        /*
        Una volta che il socket è stato configurato per l'ascolto, possiamo accettare richieste di connessione, attraverso la funzione accept. Questa funzione,
        insieme alla connect lanciata lato client costituisce indubbiamente la three way handshake. La funzione in questione accetta i seguenti parametri:

        ! 1. socket descriptor su cui siamo in ascolto
        * 2. endpoint del client, castato a puntatore a sockaddr per permetter alla accept di lavorare su qualsiasi endpoint
        ? 3. Grandezza in byte dell'endpoint passato in input

        In questo caso non vogliamo utilizzare le informazioni del client, pertanto passeremo due valori nulli. Inoltre la funzione accept ritorna un file descriptor
        che avrà le stesse proprietà del file descriptor passato in input (listen_file_descriptor)
        */
        if ((connection_file_descriptor = accept(listen_file_descriptor, (struct sockaddr *)NULL, NULL)) < 0)
        {
            /* Stampiamo un errore etichettato dalla label Accept */
            perror("Accept");
            exit(1);
        }

        /* Calcoliamo il numero di secondi rispetto al 1 gennaio 1970 */
        ticks = time(NULL);

        /* Inseriamo una stringa formattata in un buffer ed utilizziamo ctime per costruire un timestamp attuale */
        snprintf(write_buffer, sizeof(write_buffer), "%24s\n", ctime(&ticks));

        /*
        ============================================================
                        scrittura da socket descriptor
        ============================================================
        */

        /*
        Scriviamo sul file descriptor attraverso la funzione write, che accetterà in input:

        ! 1. il file descriptor su cui vogliamo scrivere
        * 2. buffer di scrittura
        ? 3. grandezza in byte del buffer
        */

        if (write(connection_file_descriptor, write_buffer, strlen(write_buffer)) != strlen(write_buffer))
        {
            /* Stampiamo un errore etichettato dalla label Write */
            perror("Write");
            exit(1);
        }

        /* Chiudiamo la connessione con il client */
        close(connection_file_descriptor);
    }
}