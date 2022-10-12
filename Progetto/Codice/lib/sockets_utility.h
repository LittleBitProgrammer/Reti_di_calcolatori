//
// Created by gaetano on 12/10/22.
//

#ifndef SOCKETS_UTILITY_H
#define SOCKETS_UTILITY_H

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

#endif // SOCKETS_UTILITY_H
