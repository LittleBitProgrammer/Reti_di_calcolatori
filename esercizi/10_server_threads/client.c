//TODO aggiungere bzero() sulle strutture
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "socket_utility.h"

#define WRITER_BUFFER_SIZE 4096
#define READER_BUFFER_SIZE 128

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
    fd_set             monitor_reader_socket;             /* Sockets to monitor while reading */
    int                max_socket_to_monitor;             /* Max socket to monitor */
    size_t             ready_sockets;                     /* Number of ready sockets */
    struct timeval     tm;                                /* Time of activity session */

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

    bzero(writer_buffer, WRITER_BUFFER_SIZE);
    bzero(reader_buffer, READER_BUFFER_SIZE);

    printf("Inserire una stringa di caratteri qualsiasi:\n");
    while(1)
    {
        tm.tv_sec = 15 * 60; /* Tempo della sessione */
        FD_ZERO(&monitor_reader_socket);
        FD_SET(STDIN_FILENO, &monitor_reader_socket);

        FD_SET(socket_file_descriptor, &monitor_reader_socket);

        max_socket_to_monitor = socket_file_descriptor > STDIN_FILENO ? socket_file_descriptor + 1 : STDIN_FILENO + 1;

        if ((ready_sockets = select(max_socket_to_monitor, &monitor_reader_socket, NULL, NULL, &tm)) < 0)
        {
            fprintf(stderr, "Errore nella select\n");
            exit(EXIT_FAILURE);
        }
        else if(ready_sockets == 0)
        {
            fprintf(stderr,"Sessione scaduta\n");
            break;
        }
        else
        {
            //printf("ready_sockets = %lu\n", ready_sockets);

            /*
             * ==================================
             * =             INPUT              =
             * ==================================
             * */
            if (FD_ISSET(STDIN_FILENO, &monitor_reader_socket))
            {
                if (fgets(writer_buffer, WRITER_BUFFER_SIZE, stdin) == NULL)
                {
                    fprintf(stderr, "Input error\n");
                    exit(EXIT_FAILURE);
                }
                /*
                 * ==================================
                 * =            REQUEST             =
                 * ==================================
                 * */
                FullWrite(socket_file_descriptor, writer_buffer, WRITER_BUFFER_SIZE);
                fflush(stdin);
            }

            /*
             * ==================================
             * =            RESPONSE            =
             * ==================================
             * */
            if (FD_ISSET(socket_file_descriptor, &monitor_reader_socket))
            {
                if(FullRead(socket_file_descriptor, reader_buffer, READER_BUFFER_SIZE) > 0)
                {
                    fprintf(stderr, "Connessione interrotta\n");
                    break;
                }
                /*
                 * ==================================
                 * =            OUTPUT              =
                 * ==================================
                 * */
                if (fputs(reader_buffer, stdout) == EOF)
                {
                    fprintf(stderr, "fputs error\n");
                    exit(EXIT_FAILURE);
                }
                /* Pulizia */
                bzero(writer_buffer, WRITER_BUFFER_SIZE);
                bzero(reader_buffer, READER_BUFFER_SIZE);

                printf("\nInserire una stringa di caratteri qualsiasi:\n");
            }
        }
    }

    close(socket_file_descriptor);
    exit(EXIT_FAILURE);
}
