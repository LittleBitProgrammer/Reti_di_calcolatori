#include <stdio.h>
#include <netinet/in.h>           /* Importata per utilizzare la struttura "@sockaddr_in" */
#include <string.h>               /* Importata per utilizzare la funzione di azzeramento dei byte di un array "@bzero()" */
#include "lib/thread_utility.h"   /* Importata per utilizzare costanti e funzioni legate alla tecnologia thread */
#include "lib/sockets_utility.h"  /* Importata per utilizzare funzioni wrapper per la gestione dei socket */

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
     * =    SOCKET CREATION     =
     * ==========================
     * */

    /* Inizializziamo il valore del file descriptor, che verrà configurato in modalità ascolto, sfruttando la funzione definita
     * nella libreria "@sockets_utilities.h". In questo modo, associamo il file descriptor a una socket
     * */
    listen_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    /* ==========================
     * =    ZEROING  ARRAYS     =
     * ==========================
     * */

    /* Azzeriamo i byte che compongono l'array "@thread_id" per evitare di avere valori raw all'interno di quest'ultimo */
    bzero(threads_id, sizeof(threads_id));

    /*
     * ==================================
     * =        SERVER CREATION         =
     * ==================================
     * */

    /* Inizializziamo i campi della struttura "@sockaddr_in" del server in modo tale costruire un Endpoint identificabile sulla rete.
     * La struttura "@sockaddr_in" è composta dai seguenti campi:
     *      @sin_family:      Famiglia degli indirizzi utilizzati (AF_INET - AF_INET6 - ecc...)
     *      @sin_port:        Porta in Network order
     *      @sin_addr.s_addr: Indirizzo IP in Network order
     * */

    /* Inizializziamo il campo famiglia della struttura "@sockaddr_in" del server con il valore "@AF_INET". In questo modo, specifichiamo
     * che il nostro server utilizzerà un indirizzo del tipo "IPv4"
     * */
    server_address.sin_family = AF_INET;

    /* Inizializziamo il campo indirizzo della struttura "@sockaddr_in" del server attraverso il valore di ritorno della funzione
     * "@htonl()" la quale accetterà come argomento la costante "@INADDR_ANY". La funzione utilizzata permette di trasformare un
     * indirizzo in formato host (ad esempio: 127.0.0.1) in formato Network order. Tale azione si rende necessaria in quanto le
     * informazioni sulla rete viaggiano in formato BIG ENDIAN, mentre alcuni host memorizzano le informazioni in formato
     * LITTLE ENDIAN, ad esempio la seguente sequenza di byte viene memorizzata su un host LITTLE ENDIAN come segue:
     *      00000001 10000000 00000000 00000000
     * Inversamente sulla rete la precedente sequenza di byte viene memorizzata in BIG ENDIAN come segue:
     *      00000000 00000000 10000000 00000001
     * Inoltre, viene passato come argomento "@INADDR_ANY". Tale costante ci permetterà di configurare il server in ascolto per
     * qualsiasi interfaccia di rete presente sull'host server.
     * Di seguito, facciamo un esempio di conversione di un indirizzo memorizzato in LITTLE ENDIAN e successivamente convertito
     * in BIG ENDIAN, supponiamo di avere il seguente indirizzo IP:
     * 127.0.0.1
     * Convertiamo l'indirizzo in gruppi di 8 bit:
     * 01111111 00000000 00000000 00000001
     * Invertiamo l'ordine dei byte secondo le specifiche del BIG ENDIAN:
     * 00000001 00000000 00000000 01111111
     * Otteniamo il valore decimale BIG ENDIAN:
     * 16777343
     * */
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Inizializziamo il campo porta della struttura "@sockaddr_in" del server attraverso il valore di ritorno della funzione
     * "@htons()" la quale accetterà come argomento un intero rappresentante la porta desiderata su cui il server deve rimanere
     * in ascolto. Le porte sono interi a 16 bit da 0 a 65535, raggruppate nel seguente modo:
     *      - da 0 a 1023, porte riservate ai processi root;
     *      - da 1024 a 49151, porte registrate;
     *      - da 49152 a 65535, porte effimere, per i client, ai quali non interessa scegliere una porta specifica.
     * Per il progetto si è deciso di utilizzare una porta registrata "6463"
     * */
    server_address.sin_port = htons(6463);

    


    return 0;
}