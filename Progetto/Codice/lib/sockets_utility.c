//
// Created by gaetano on 12/10/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
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

    /* In caso di successo, la funzione "@socket()" ritornerà un valore maggiore o uguale di 0, ovvero il valore del
     * file descriptor associata alla socket creata. In caso di errore viene ritornato -1 ed errno (numero dell'ultimo errore)
     * viene configurato per indicare quest'ultimo */
    if((file_descriptor = socket(address_family, transport_type, transport_subtype)) < 0)
    {
        /* La seguente funzione produce un messaggio sullo standard error (file descriptor: 2) che descrive la natura dell'errore */
        perror("Creation socket error: ");
        exit(EXIT_FAILURE);
    }

    return file_descriptor;
}
