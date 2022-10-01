#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "socket_utility.h"

#define WRITER_BUFFER_SIZE 4096
#define READER_BUFFER_SIZE 3

int main(int argc, char**argv) {
    /*
     * ==========================
     * =       Variables        =
     * ==========================
     * */
    int                socket_file_descriptor;            /* socket client */
    struct sockaddr_in server_address;                    /* Endpoint server */
    struct hostent*    server_dns;                        /* structure dns of server */
    char               writer_buffer[WRITER_BUFFER_SIZE]; /* Reader buffer */
    char               reader_buffer[READER_BUFFER_SIZE]; /* writer buffer */

    /*
     * ==========================
     * =       Arguments        =
     * ==========================
     * */
    if(argc != 2)
    {
        fprintf(stderr, "usage: %s <IP/HOSTNAME>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * ==========================
     * =          DNS           =
     * ==========================
     * */
    if((server_dns = gethostbyname(argv[1])) == NULL)
    {
        herror("Errore DNS client");
        exit(EXIT_FAILURE);
    }

    /*
     * ==================================
     * =        SOCKET CREATION         =
     * ==================================
     * */
    socket_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    /*
     * ==================================
     * =        SERVER CREATION         =
     * ==================================
     * */
    server_address.sin_family = server_dns->h_addrtype;
    server_address.sin_port = htons(13);
    server_address.sin_addr = *((struct in_addr *)server_dns->h_addr_list[0]);

    /*
     * ==================================
     * =           CONNECTION           =
     * ==================================
     * */
    ConnectIPV4(socket_file_descriptor, &server_address);

    /*
     * ==================================
     * =             INPUT              =
     * ==================================
     * */
    printf("Inserire una stringa di caratteri qualsiasi:\n");
    if(fgets(writer_buffer, WRITER_BUFFER_SIZE, stdin) == NULL)
    {
        fprintf(stderr, "Input error\n");
        close(socket_file_descriptor);
        exit(EXIT_FAILURE);
    }

    /*
     * ==================================
     * =            REQUEST             =
     * ==================================
     * */
    FullWrite(socket_file_descriptor, writer_buffer, WRITER_BUFFER_SIZE);

    /*
     * ==================================
     * =            RESPONSE            =
     * ==================================
     * */
    FullRead(socket_file_descriptor, reader_buffer, READER_BUFFER_SIZE);

    /*
     * ==================================
     * =            OUTPUT              =
     * ==================================
     * */
    if(fputs(reader_buffer, stdout) == EOF)
    {
        fprintf(stderr, "fputs error\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
