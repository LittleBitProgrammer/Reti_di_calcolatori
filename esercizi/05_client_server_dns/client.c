#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "socket_utility.h"

#define BUFFER_SIZE 1025

int main(int argc, char **argv)
{
    int socket_file_descriptor;
    struct sockaddr_in server_address;
    struct hostent *serverDNS;

    char buffer[BUFFER_SIZE];

    /* Controllo degli argomenti inseriti */
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <IP ADDRESS>\n", argv[0]);
        exit(1);
    }

    if((serverDNS = gethostbyname(argv[1])) == NULL)
    {
        herror("Errore di risoluzione diretta");
        exit(EXIT_FAILURE);
    }

    /* Creazione della socket */
    socket_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    /* Popolazione ENDPOINT server */
    server_address.sin_family = serverDNS->h_addrtype;
    server_address.sin_port = htons(13);

    if((inet_aton(serverDNS->h_addr_list[0], &server_address.sin_addr)) < 0)
    {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Connessione */
    ConnectIPV4(socket_file_descriptor, &server_address);
    FullRead(socket_file_descriptor, buffer, BUFFER_SIZE);

    /* Stampa a video */
    if (fputs(buffer, stdout) == EOF)
    {
        fprintf(stderr, "fputs error\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
