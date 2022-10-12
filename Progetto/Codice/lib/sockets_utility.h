//
// Created by gaetano on 12/10/22.
//

#ifndef SOCKETS_UTILITY_H
#define SOCKETS_UTILITY_H

#include <arpa/inet.h>              /* Importata per utilizzare le strutture di tipo "@sockaddr_in" */
#define DEFAULT_BACKLOG_SIZE 50     /* Costante volta a definire la dimensione di default massima delle backlog dei
                                     * servers definiti nel sistema GreenPass */

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

/**
 * @brief La funzione configura "@file_descriptor", passato in input, in ascolto con una coda delle richieste pari a "@backlog_size"
 *
 * @param file_descriptor File descriptor del socket da configurare in modalità ascolto
 * @param backlog_size Coda delle connessioni in pendenza
 * */
void Listen(int, int);

/**
 * @brief La funzione esegue la Three way Handshake con il client che ne fa espressamente richiesta
 *
 * @param listen_file_descriptor File descriptor configurato in modalità ascolto sul quale arriverà la nuova richiesta di connessione
 * @param client_address Parametro di output, utile ad identificare l'indirizzo del client
 * @param client_size Parametro di output rappresentante la dimensione della struttura utilizzata per rappresentare il client
 *
 * @return File descriptor avente le stesse proprietà del "@listen_file_descriptor" utile a gestire la nuova connessione e a servire il client
 * */
int AcceptIPV4(int, struct sockaddr_in*, socklen_t*);

#endif // SOCKETS_UTILITY_H
