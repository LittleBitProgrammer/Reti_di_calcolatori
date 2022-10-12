//
// Created by gaetano on 12/10/22.
//

#ifndef SOCKETS_UTILITY_H
#define SOCKETS_UTILITY_H

#include <arpa/inet.h>    /* Importata per utilizzare le strutture di tipo "@sockaddr_in" */

/**
 * @brief Funzione che permette la creazione di un file descriptor associato ad una socket
 *
 * @param address_family Famiglia degli indirizzi utilizzati (IPv4 - IPv6 - ecc...)
 * @param transport_type Protocollo di trasporto utilizzato (TCP - UDP)
 * @param transport_subtype Sottotipo del protocollo di trasporto (solitamente configurato a 0)
 *
 * @return File descriptor associato alla socket creata
 * */
int Socket(int, int, int);

/**
 * @brief Questa funzione serve ad asscoiare un Endpoint ad un file descriptor specifico "@file_descriptor_to_bind" e "@endpoint"
 *
 * @param file_descriptor_to_bind File descrpitor del socket da associare ad una struttura di tipo "@sockaddr_in"
 * @param endpoint Indirizzo da associare al socket "@file_descriptor_to_bind"
 * */
void BindIPV4(int, struct sockaddr_in*);

#endif // SOCKETS_UTILITY_H
