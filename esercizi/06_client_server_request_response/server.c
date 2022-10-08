//
// Created by roberto on 30/09/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "socket_utility.h"

#define WRITER_BUFFER_SIZE 128
#define READER_BUFFER_SIZE 4096

#define BACKLOG 1024

int main()
{
    /*
     * ==========================
     * =       Variables        =
     * ==========================
     * */
    int                listen_file_descriptor;               /* Socket in listening */
    int                connection_file_descriptor;           /* Socket for the connection with client */
    int                is_address_reusable = 1;              /* Flag for socket option */
    struct sockaddr_in server_address;                       /* Structure for the server */
    struct sockaddr_in client_address;                       /* Structure for the client */
    char               writer_buffer[WRITER_BUFFER_SIZE];    /* Reader buffer */
    char               reader_buffer[READER_BUFFER_SIZE];    /* Writer buffer */
    char               ip_buffer[INET6_ADDRSTRLEN];          /* Buffer of IP address */
    socklen_t          client_size = sizeof(client_address); /* Size del client */
    pid_t              process_id;                           /* ID of forked process */

    /*
     * ==================================
     * =        SOCKET CREATION         =
     * ==================================
     * */
    listen_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    /*
     * ==================================
     * =        SERVER CREATION         =
     * ==================================
     * */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(13);

    /*
     * ==================================
     * =         SOCKET OPTION          =
     * ==================================
     * */
    setsockopt(listen_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &is_address_reusable, sizeof(is_address_reusable));

    /*
     * ==================================
     * =               BIND             =
     * ==================================
     * */
    BindIPV4(listen_file_descriptor, &server_address);

    /*
     * ==================================
     * =              LISTEN            =
     * ==================================
     * */
    Listen(listen_file_descriptor, BACKLOG);

    /*
     * ==================================
     * =             ZOMBIE             =
     * ==================================
     * */
    signal(SIGCHLD, SIG_IGN);

    /*
     * ==================================
     * =             ENDLESS            =
     * ==================================
     * */
    for(;;)
    {
        /*
         * ==================================
         * =             ACCEPT             =
         * ==================================
         * */
        connection_file_descriptor = AcceptIPV4(listen_file_descriptor, &client_address, &client_size);

        /*
         * ==================================
         * =              FORK              =
         * ==================================
         * */
        if((process_id = fork()) < 0)
        {
            perror("Fork error: ");
            exit(EXIT_FAILURE);
        }

        /*
         * ==================================
         * =             CHILD              =
         * ==================================
         * */
        if(process_id == 0)
        {
            /*
             * ==================================
             * =          CLOSE LISTEN          =
             * ==================================
             * */
            close(listen_file_descriptor);

            while(1)
            {
                /*
                 * ==================================
                 * =          PRINT CLIENT          =
                 * ==================================
                 * */
                PrintClientIPV4(&client_address, ip_buffer, INET6_ADDRSTRLEN);

                /*
                 * ==================================
                 * =           FULL READ            =
                 * ==================================
                 * */
                if(FullRead(connection_file_descriptor, reader_buffer, READER_BUFFER_SIZE) > 0)
                {
                    break;
                }

                snprintf(writer_buffer, WRITER_BUFFER_SIZE, "%lu\n", strlen(reader_buffer) - 1);

                /*
                 * ==================================
                 * =           FULL WRITE           =
                 * ==================================
                 * */
                FullWrite(connection_file_descriptor, writer_buffer, WRITER_BUFFER_SIZE);
            }

            /*
             * ==================================
             * =       CLOSE CONNECTION         =
             * ==================================
             * */
            close(connection_file_descriptor);

            exit(EXIT_SUCCESS);
        }
        else
        {
            /*
             * ==================================
             * =       CLOSE CONNECTION         =
             * ==================================
             * */
            close(connection_file_descriptor);
        }
    }

    /* Irraggiungibile */
    exit(EXIT_SUCCESS);
}
