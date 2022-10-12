//
// Created by gaetano on 12/10/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "sockets_utility.h"

/**
 * @brief Funzione che permette la creazione di un file descriptor associato ad una socket
 *
 * @param address_family Famiglia degli indirizzi utilizzati (IPv4 - IPv6 - ecc...)
 * @param transport_type Protocollo di trasporto utilizzato (TCP - UDP)
 * @param transport_subtype Sottotipo del protocollo di trasporto (solitamente configurato a 0)
 *
 * @return File descriptor associato alla socket creata
 * */
int Socket(int address_family, int transport_type, int transport_subtype)
{
    /* File descriptor da inizializzare al valore di ritorno della chiamata "@socket()" */
    int file_descriptor;

    /*
     * In caso di successo, la funzione "@socket()" ritornerà un valore maggiore o uguale di 0, ovvero il valore del
     * file descriptor associato alla socket creata. In caso di errore viene ritornato -1 ed errno (numero dell'ultimo errore)
     * viene configurato per indicare quest'ultimo
     * */
    if((file_descriptor = socket(address_family, transport_type, transport_subtype)) < 0)
    {
        /* La seguente funzione produce un messaggio sullo standard error (file descriptor: 2) che descrive la natura dell'errore */
        perror("Creation socket error: ");
        exit(EXIT_FAILURE);
    }

    return file_descriptor;
}

/**
 * @brief Questa funzione serve ad asscoiare un Endpoint ad un file descriptor specifico "@file_descriptor_to_bind" e "@endpoint"
 *
 * @param file_descriptor_to_bind File descrpitor del socket da associare ad una struttura di tipo "@sockaddr_in"
 * @param endpoint Indirizzo da associare al socket "@file_descriptor_to_bind"
 * */
void BindIPV4(int file_descriptor_to_bind, struct sockaddr_in* endpoint)
{
    /*
     * Attraverso la funzione "@bind()" associamo un file descriptor a un endpoint di tipo "@sockaddr", in modo tale da ereditare
     * le caratteristiche dell'indirizzo rappresentato dalla struttura passata come secondo argomento. In particolare sfruttiamo
     * il valore di ritorno della "@bind()" per la gestione degli errori, in quanto, quest'ultima ritornerà 0 in caso di successo
     * oppure -1 in caso di errore, configurando errno (numero dell'ultimo errore) per indicare l'errore
     * */
    if(bind(file_descriptor_to_bind, (struct sockaddr*)endpoint, sizeof(*endpoint)) < 0)
    {
        /* La seguente funzione produce un messaggio sullo standard error (file descriptor: 2) che descrive la natura dell'errore */
        perror("Bind to endpoint error: ");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief La funzione configura "@file_descriptor", passato in input, in ascolto con una coda delle richieste pari a "@backlog_size"
 *
 * @param file_descriptor File descriptor del socket da configurare in modalità ascolto
 * @param backlog_size Coda delle connessioni in pendenza
 * */
void Listen(int file_descriptor, int backlog_size)
{
    /*
     * Attraverso la funzione "@listen()" configuriamo il socket, identificato dal "@file_descriptor" passato in input, in modalità
     * ascolto. In questo modo il socket sarà pronto per accettare nuove connessioni. In particolare sfruttiamo
     * il valore di ritorno della "@listen()" per la gestione degli errori, in quanto, quest'ultima ritornerà 0 in caso di successo
     * oppure -1 in caso di errore, configurando errno (numero dell'ultimo errore) per indicare l'errore
     * */
    if(listen(file_descriptor, backlog_size) < 0)
    {
        /* La seguente funzione produce un messaggio sullo standard error (file descriptor: 2) che descrive la natura dell'errore */
        perror("Error while configuring the socket in listening: ");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief La funzione esegue la Three way Handshake con il client che ne fa espressamente richiesta
 *
 * @param listen_file_descriptor File descriptor configurato in modalità ascolto sul quale arriverà la nuova richiesta di connessione
 * @param client_address Parametro di output, utile ad identificare l'indirizzo del client
 * @param client_size Parametro di output rappresentante la dimensione della struttura utilizzata per rappresentare il client
 *
 * @return File descriptor avente le stesse proprietà del "@listen_file_descriptor" utile a gestire la nuova connessione e a servire il client
 * */
int AcceptIPV4(int listen_file_descriptor, struct sockaddr_in* client_address, socklen_t* client_size)
{
    /* File descriptor da inizializzare al valore di ritorno della chiamata "@accept()" */
    int connection_file_descriptor;

    /*
     * Attraverso la seguente funzione procediamo con l'eseguire e finalizzare la procedura Three way Handshake. In questo modo
     * andiamo a stabilire una connessione con il client che ne ha fatto richiesta. Inoltre, sfruttiamo il secondo e il terzo parametro
     * per ritornare le informazioni inerenti al client. Ritornerà un valore maggiore o uguale di 0, ovvero il valore del
     * file descriptor associato alla socket. In caso di errore viene ritornato -1 ed errno (numero dell'ultimo errore)
     * viene configurato per indicare quest'ultimo
     * */
    if((connection_file_descriptor = accept(listen_file_descriptor, (struct sockaddr *)client_address, client_size)) < 0)
    {
        /* La seguente funzione produce un messaggio sullo standard error (file descriptor: 2) che descrive la natura dell'errore */
        perror("Error while trying to accept a new connection: ");
        exit(EXIT_FAILURE);
    }

    return connection_file_descriptor;
}