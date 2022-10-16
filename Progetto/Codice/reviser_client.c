#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lib/sockets_utility.h"
#include "lib/menu_utility.h"
#include "lib/package_utility.h"

int main(int argc, char **argv)
{
    /* ==========================
     * =       Variables        =
     * ==========================
     */

    int                client_file_descriptor;
    struct sockaddr_in server_address;
    struct hostent*    server_dns;
    char               command_writer_buffer[CMD_BUFFER_LEN];
    char               writer_buffer[21];
    Reviser_package    reviser_package;

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
    bzero(command_writer_buffer, CMD_BUFFER_LEN);
    bzero(writer_buffer, 21);

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
    server_address.sin_port = htons(6465);

    /*
     * ====================
     * =       MENU       =
     * ====================
     * */
    if(!run_reviser_menu(writer_buffer))
    {
        /* Chiusura del socket file descriptor connesso al server */
        close(client_file_descriptor);
        /* Terminiamo con successo il processo client */
        exit(EXIT_FAILURE);
    }

    /*
     * ==================================
     * =           CONNECTION           =
     * ==================================
     * */

    /* Eseguiamo una richiesta di Three way Handshake alla struttura "@sockaddr_in" del server precedentemente generata */
    ConnectIPV4(client_file_descriptor, &server_address);

    /*
     * ==================================
     * =          CMD_REQUEST           =
     * ==================================
     * */

    strcpy(command_writer_buffer, "CMD_REV");
    FullWrite(client_file_descriptor, command_writer_buffer, CMD_BUFFER_LEN);

    /*
     * ==================================
     * =        REVISOR REQUEST         =
     * ==================================
     * */

    FullWrite(client_file_descriptor, writer_buffer, 21);
    if(FullRead(client_file_descriptor, &reviser_package, sizeof(reviser_package)) > 0)
    {
        /* Chiusura del socket file descriptor connesso al server */
        close(client_file_descriptor);
        /* Terminiamo con successo il processo client */
        exit(EXIT_FAILURE);
    }

    printf("Informazioni:\n\n");
    if(reviser_package.file_flags.open_file_flag || reviser_package.file_flags.write_file_flag)
    {
        fprintf(stderr,"Anomalia durante l'operazione del server\n");
    }
    else
    {
        printf("\n- Validità:\t Green pass %s\n",    reviser_package.is_green_pass_valid ? "non valido" : "valido");
        printf("- Scadenza:\t %d/%d/%d\n",           reviser_package.expiration_date.tm_mday, 
                                                     reviser_package.expiration_date.tm_mon + 1,
                                                     reviser_package.expiration_date.tm_year + 1900);
        printf("- Motivazione:\t %s\n",              reviser_package.motivation);
        printf("- Ultimo Aggiornamento: %d/%d/%d\n", reviser_package.last_update.tm_mday, 
                                                     reviser_package.last_update.tm_mon + 1,
                                                     reviser_package.last_update.tm_year + 1900);
    }

    /* Chiusura del socket file descriptor connesso al server */
    close(client_file_descriptor);
    /* Terminiamo con successo il processo client */
    exit(EXIT_SUCCESS);
}