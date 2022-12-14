#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include "socket_utility.h"

/**
 * @brief Crea un file descriptor associato ad una socket
 * 
 * @param address_family Famiglia degli indirizzi utilizzati
 * @param transport_type Protocollo di trasporto utilizzato
 * @param transport_subtype Sottotipo del protocollo di trasporto
 * @return int file descriptor della socket creata
 */
int Socket(int address_family, int transport_type, int transport_subtype)
{
    int file_descriptor;

    if((file_descriptor = socket(address_family, transport_type, transport_subtype))< 0)
    {
        perror("Socket");
        exit(1);
    }

    return file_descriptor;
}

/**
 * @brief Connette due socket (@source_file_descriptor e @destination endpoint)
 * 
 * @param source_file_descriptor file descrittore della sorgente da connettere
 * @param destination_endpoint endpoint della destinazione da connettere
 */
void ConnectIPV4(int source_file_descriptor, struct sockaddr_in *destination_endpoint)
{
    if(connect(source_file_descriptor, (struct sockaddr*) destination_endpoint, sizeof(*destination_endpoint)) < 0)
    {
        perror("Connect IPV4");
        exit(1);
    }
}

/**
 * @brief Collega un indirizzo con un endpoint (@file_descriptor_to_bind e @endpoint)
 * 
 * @param file_descriptor_to_bind file descrittore del socket
 * @param endpoint indirizzo da associare al socket
 */
void BindIPV4(int file_descriptor_to_bind, struct sockaddr_in *endpoint)
{
    if(bind(file_descriptor_to_bind, (struct sockaddr*) endpoint, sizeof(*endpoint)) < 0)
    {
        perror("Bind IPV4");
        exit(1);
    }
}

/**
 * @brief Configura il file descrittore passato in input in ascolto con una coda delle richieste pari a @max_len_request 
 * 
 * @param listen_file_descriptor file descriptor su cui vogliamo rimanere in ascolto
 * @param max_len_request_queue numero massimo di client in attesa di essere serviti
 */
void Listen(int listen_file_descriptor, int max_len_request_queue)
{
    if(listen(listen_file_descriptor, max_len_request_queue) < 0)
    {
        perror("Listen");
        exit(1);
    }
}

/**
 * @brief Accetta una connessione in pendenza
 * 
 * @param listen_file_descriptor  File descriptor su cui arriver?? la connessione (deve essere configurato in modalit?? listen)
 * @param client_endpoint puntatore alla struttura per rappresentare un endpoint (parametro di output)
 * @param size_client_endpoint puntatore alla size della struttura utilizzata (parametro di output)
 * @return int file descriptor con le stesse propriet?? del @listen_file_descriptor
 */
int AcceptIPV4(int listen_file_descriptor, struct sockaddr_in *client_endpoint, socklen_t *size_client_endpoint)
{
    int connection_file_descriptor;

    if((connection_file_descriptor = accept(listen_file_descriptor, (struct sockaddr*)client_endpoint, size_client_endpoint)) < 0)
    {
        perror("Accept");
        exit(1);
    }

    return connection_file_descriptor;
}

/**
 * @brief funzione per la scrittura di caratteri in un buffer
 *
 * @param file_descriptor file descrittore del socket su cui scrivere
 * @param buffer area di memoria in cui scrivere
 * @param ch_to_print numero massimo di caratteri da scrivere nel buffer
 * @return ritorna il valore dei caratteri rimanenti da scrivere
 */
size_t FullWrite(int file_descriptor, const void *buffer, size_t ch_to_print)
{
    size_t n_left = ch_to_print; /* Caratteri rimanenti da scrivere */
    ssize_t n_written; /* Caratteri scritti */

    /* Costrutto iterativo, che si ripeter?? fino a quando ci sono caratteri da scrivere */
    while(n_left > 0)
    {
        /* Se la write restituisce errore */
        if((n_written = write(file_descriptor,buffer,n_left)) < 0)
        {
            /* Interruzione da una system call */
            if(errno == EINTR)
            {
                /* Ripeti il loop */
                continue;
            }
            else
            {
                /* In qualsiasi altro caso esci con errore */
                exit((int)n_written); /* n_written = -1 */
            }
        }

        /* Aggiorniamo i caratteri rimanenti da scrivere */
        n_left -= n_written;
        /* Aggiorniamo lo spiazzamento del buffer */
        buffer += n_written;
    }

    return (n_left);
}

/**
 * @brief funzione per la lettura dei caratteri di un buffer
 *
 * @param file_descriptor file descrittore del socket su cui leggere
 * @param buffer buffer che deve essere letto
 * @param ch_to_read caratteri da leggere dal buffer
 * @return il valore dei caratteri rimanenti da leggere
 */
size_t FullRead(int file_descriptor, void *buffer, size_t ch_to_read)
{
    size_t n_left = ch_to_read; /* Caratteri rimanenti da leggere */
    ssize_t n_read; /* Caratteri letti */

    /* Costrutto iterativo, che si ripeter?? fino a quando ci sono caratteri da leggere */
    while(n_left > 0)
    {
        /* Se la read restituisce errore */
        if((n_read = read(file_descriptor, buffer, n_left)) < 0)
        {
            /* Interruzione da una system call */
            if(errno == EINTR)
            {
                continue;
            }
            else
            {
                /* In qualsiasi altro caso esci con errore */
                exit((int)n_read);/* n_read = -1 */
            }
        }
        else if(n_read == 0)    /* EOF */
        {
            /* Se abbiamo raggiunto la fine del file, interrompiamo il ciclo */
            break;
        }

        /* Aggiorniamo i caratteri rimanenti da leggere */
        n_left -= n_read;
        /* Aggiorniamo lo spiazzamento del buffer */
        buffer += n_read;
    }

    buffer = 0;
    return(n_left);
}

/**
 * @brief Funzione che permette di stampare le informazioni relative al client
 *
 * @param client_address Endpoint del client contente le informazioni da stampare
 * @param buffer Buffer utilizzato per memorizzare le informazioni del client
 * @param buffer_size Grandezza del buffer
 * */
void PrintClientIPV4(struct sockaddr_in* client_address, char *buffer, socklen_t buffer_size)
{
    struct hostent *host;
    inet_ntop(AF_INET,&(client_address->sin_addr),buffer,buffer_size);

    /* Stampa informazioni del client */
    printf("Request from host %s, port %d,", buffer, ntohs(client_address->sin_port));

    if((host = gethostbyaddr((const char *) &(client_address->sin_addr), sizeof(client_address->sin_addr), client_address->sin_family)) == NULL)
    {
        herror("Reverse DNS error: ");
        exit(EXIT_FAILURE);
    }


    printf(" hostname %s\n", host->h_name);
}

bool check_connection(int socket_file_description)
{
    bool is_connected;

    if((is_connected = read(socket_file_description, NULL, 1)) == 0)
    {
        printf("Connessione interrotta\n");
        exit(EXIT_FAILURE);
    }

    return is_connected;
}
