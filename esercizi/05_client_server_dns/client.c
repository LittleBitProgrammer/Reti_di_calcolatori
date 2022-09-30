#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "socket_utility.h"

#define BUFFER_SIZE 1025

int main(int argc, char **argv)
{
    int socket_file_descriptor;
    struct sockaddr_in server_address;
    struct hostent *server_dns;

    char buffer[BUFFER_SIZE];

    /* Controllo degli argomenti inseriti */
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <IP ADDRESS>\n", argv[0]);
        exit(1);
    }

    if((server_dns = gethostbyname(argv[1])) == NULL)
    {
        herror("Errore di risoluzione diretta");
        exit(EXIT_FAILURE);
    }

    /* Creazione della socket */
    socket_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    /* Popolazione ENDPOINT server */
    server_address.sin_family = server_dns->h_addrtype;
    server_address.sin_port = htons(13);
    server_address.sin_addr = *((struct in_addr *)server_dns->h_addr_list[0]);

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
