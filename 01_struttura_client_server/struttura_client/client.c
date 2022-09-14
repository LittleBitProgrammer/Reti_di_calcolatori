/**
 * @file client.c
 * @author Roberto Vecchio
 * @brief Tale programma ha lo scopo di creare una struttura di tipo client. La sintassi utilizzata per lanciare questo programma è la seguente:
 *        ./nome_programma <IPaddress>
 *        dove IPaddress è l'indirizzo IP a cui ci vogliamo collegare, attraverso un processo client
 * 
 *        Passi creazione client:
 *        
 *        1. Controllo parametri in input della funzione main
 *        2. Creazione socket, attraverso la funzione socket() della libreria sys/socket.h
 *        3. Costruzione endpoint (server)
 *        4. Connessione del descrittore Socket al server
 *        5. Lettura da socket descriptor
 * 
 * @version 1.0
 * @date 2022-09-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Import delle librerie */
#include <stdio.h> /* fprintf & fputs */
#include <stdlib.h> /* exit*/
#include <unistd.h> /* write & read*/
#include <sys/socket.h> /* socket, connect*/
#include <arpa/inet.h> /* sockaddr_in, htons, inet_pton */

/* Main */
int main(int argc, char **argv)
{
    /* 
    ======================================
           Dichiarazione Variabili 
    ======================================
    */

    /*----- Socket ----- */
    int sockfd; /* Descrittore del socket */

    /*-----  Lettura ----- */
    char recvline[1025]; /* Buffer di lettura */
    int n; /* Intero utilizzato per la lettura */

    /*----- Connessione server -----*/
    struct sockaddr_in servaddr; /* Utile per specificare l'indirizzo del server e la porta del processo, a cui ci si vuole connettere */

    /*
    ============================================================================
           Controlliamo la validità dell'input della linea di comando 
    ============================================================================
    */

    /* Se il programma viene eseguito passando in input 0 oppure più argomenti, mostriamo un errore sullo standard output */
    if(argc != 2)
    {
        /* Quando il main viene eseguito, lo standard output, input ed error vengono automaticamente aperti, quindi è possibile eseguire direttamente una fprintf */
        fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }

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
    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        fprintf(stderr,"socket error\n");
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
   ? - Il campo sin_addr deve essere inizializzato attraverso la funzione inet_pton(int <famiglia_indirizzi>, char *<stringa_ip_ddd.ddd.ddd.ddd>, struct in_addr *<indirizzo_tipo>)
   ?   in quanto l'indirizzo deve essere codificato in un valore numerico a 32 bit ed eventualmente deve essere codificato in big endian nel caso in cui venga eseguito il 
   ?   programma su host little_endian. La codifica in valore numerico viene effettuata trasformando in bit i 4 ottetti che compongono l'indirizzo IP (caso IPV4)
   */
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(13);
   if (inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0)
   {
    fprintf(stderr, "inet_pton error for %s\n", argv[1]);
    exit(1);
   }

   /*
    ============================================================
            Connessione del descrittore Socket al server
    ============================================================
    */
   
   /*
   Per Connettere un descrittore Socket ad un server abbiamo bisogno di utilizzare la funzione connect. Quest'ultima accetterà i seguenti parametri di intput:

   ! - Descrittore di file del client (int <nome_descrittore>)
   * - Puntatore a sockaddr, in quanto sockaddr_in riesce a rappresentare un indirizzo internet IPV4, mentre la libreria socket di Berkley lavora su qualsiasi 
   *   tipo di indirizzo. Viene quindi eseguito un cast per rispettare tale generalità. ((struct sockaddr *) <nome_server>)
   ? - La connect ha bisogno di capire su che tipo di struttura dati sta lavorando (IPV4, IPV6 ecc..), pertanto in input viene anche passato un unsigned int, corrispondente 
   ?   alla size del servaddr  (sizeof(<name_server>))

   Se la connect restituisce un valore minore di 0, allora la connessione tra client e server non sarà andata a buon fine. Di seguito gestiamo anche tale eventualità
   */
   if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
   {
    fprintf(stderr, "Connect error\n");
    exit(1);
   }

   /*
    ============================================================
                    Lettura da socket descriptor
    ============================================================
    */

   /*
   Per effettuare la lettura delle informazioni inviateci dal server, occore eseguire una read dal file descriptor della socket creata dal nostro processo client.
   In quanto i socket TCP, una volta che la connessione TCP è instaurata, sono accessibili come se fossero dei file mediante un descrittore di file (un intero), ottenuto 
   tramite la funzione socket precedentemente eseguita.

   In particolare la read restituisce:

   ! - I byte letti dal flusso in entrata
   * - 0 (end-of-file) che starà ad indicare che l'altro endpoint ha volutamente chiuso la connessione e quindi il socket non potrà più essere utilizzato per leggere
   ? - (-1) se accade un errore

   Inoltre tale funzione accetterà in input:

   ! - file descriptor da cui leggere
   * - buffer su cui memorizzare i dati i lettura
   ? - numero di byte da leggere
   */
   while((n = read(sockfd,recvline,1024)) > 0)
   {
    /* Aggiungiamo il carattere terminazione ad ogni lettura */
    recvline[n] =  0;

    /* Stampiamo a video quanto letto */
    if (fputs(recvline,stdout) == EOF)
    {
        /* Gestiamo errore fputs */
        fprintf(stderr,"fputs error\n");
        exit(1);
    }
   }

   /* Gestiamo errore funzione read */
   if(n < 0)
   {
    fprintf(stderr, "read error\n");
    exit(1);
   }
   
   /* 
   --> N = 0 <--
   In questo caso non ci sono più caratteri da leggere, pertanto usciremo dal programma richiamando una exit(0), ovvero uscita con successo.
   Inoltre la funzione exit, ci permette di chiudere tutti i file descriptor, compreso quello associato al socket, pertanto, verrà richiamata automaticamente la 
   close() del socket.
   */
   exit(0);
}