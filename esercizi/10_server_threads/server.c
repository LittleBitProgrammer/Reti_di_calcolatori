//
// Created by roberto on 30/09/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include "socket_utility.h"
#include "thread_utility.h"

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
    int                 listen_file_descriptor;                /* Socket in listening */
    int                 connection_file_descriptor;            /* Socket for the connection with client */
    int                 is_address_reusable = 1;               /* Flag for socket option */
    struct sockaddr_in  server_address;                        /* Structure for the server */
    struct sockaddr_in  client_address;                        /* Structure for the client */
    socklen_t           client_size = sizeof(client_address);  /* Size del client */

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
    bzero(&server_address, sizeof(server_address));

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

    int i;
    pthread_t thread_id[MAX_THREAD];

    /*
     * ==================================
     * =            ENDLESS             =
     * ==================================
     * */
    for(;;)
    {
        connection_file_descriptor = AcceptIPV4(listen_file_descriptor, &client_address, &client_size);

        if(pthread_create(&thread_id[i++], NULL, server_handler, &connection_file_descriptor) != 0)
        {
            fprintf(stderr, "Failed to create thread \n");
            break;
        }

        if(i >= 50)
        {
            i = 0;
            while (i < 50)
            {
                if (pthread_join(thread_id[i++], NULL) != 0)
                {
                    fprintf(stderr, "Failed to join thread \n");
                    break;
                }
            }
            i = 0;
        }
    }

    /* Irraggiungibile */
    exit(EXIT_FAILURE);
}









