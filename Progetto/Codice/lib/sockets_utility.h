#ifndef SOCKETS_UTILITY_H
#define SOCKETS_UTILITY_H

#include <arpa/inet.h>              /* Importata per utilizzare le strutture di tipo "@sockaddr_in" */
#define CMD_BUFFER_LEN 8            /* Costante volta a definire la dimensione massima dei comandi inviati */
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

/**
 * @brief La funzione permette la lettura dei byte presenti in un "@buffer" anche in caso di interruzioni da parte di una
 *        System call. In questo modo, gestiamo le interruzioni permettendo alla "@read()" di leggere tutti i byte presenti
 *        nel "@buffer"
 *
 * @param file_descriptor File descriptor del socket su cui eseguire l'operazione di lettura attraverso una "@read()"
 * @param buffer Buffer contenente i byte da leggere
 * @param n_bytes Numero di bytes presenti nel "@buffer"
 *
 * @return Numero di byte rimanenti da leggere
 * */
size_t FullRead(int, void*, size_t);

/**
 * @brief La funzione permette la scrittura dei byte presenti in un "@buffer" anche in caso di interruzioni da parte di una
 *        System call. In questo modo, gestiamo le interruzioni permettendo alla "@write()" di scrivere tutti i byte presenti
 *        nel "@buffer"
 *
 * @param file_descriptor File descriptor del socket su cui eseguire l'operazione di scrittura attraverso una "@write()"
 * @param buffer Buffer contenente i byte da scrivere
 * @param n_bytes Numero di bytes presenti nel "@buffer"
 *
 * @return Numero di byte rimanenti da scrivere
 * */
size_t FullWrite(int, void*, size_t);

#endif // SOCKETS_UTILITY_H
