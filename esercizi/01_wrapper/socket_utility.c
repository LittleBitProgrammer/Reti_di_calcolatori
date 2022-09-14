#include <stdio.h>
#include <stdlib.h>
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
 * @param destination_endpoint file descrittore della destinazione da connettere
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
 * @param listen_file_descriptor 
 * @param max_len_request_queue 
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
 * @brief 
 * 
 * @param listen_file_descriptor 
 * @param client_endpoint 
 * @param size_client_endpoint 
 * @return int 
 */
int Accept(int listen_file_descriptor, struct sockaddr *client_endpoint, socklen_t *size_client_endpoint)
{
    int connection_file_descriptor;

    if((connection_file_descriptor = accept(listen_file_descriptor, client_endpoint, size_client_endpoint)) < 0)
    {
        perror("Accept");
        exit(1);
    }

    return connection_file_descriptor;
}