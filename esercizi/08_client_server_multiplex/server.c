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
#include "number_utility.h"

#define WRITER_BUFFER_SIZE 128
#define READER_BUFFER_SIZE 4096
#define MAX_SOCKETS 64

#define BACKLOG 1024

int main()
{
    /*
     * ==========================
     * =       Variables        =
     * ==========================
     * */
    int                listen_file_descriptor;                /* Socket in listening */
    int                connection_file_descriptor;            /* Socket for the connection with client */
    int                is_address_reusable = 1;               /* Flag for socket option */
    struct sockaddr_in server_address;                        /* Structure for the server */
    struct sockaddr_in client_address;                        /* Structure for the client */
    char               writer_buffer[WRITER_BUFFER_SIZE];     /* Reader buffer */
    char               reader_buffer[READER_BUFFER_SIZE];     /* Writer buffer */
    char               ip_buffer[INET6_ADDRSTRLEN];           /* Buffer of IP address */
    socklen_t          client_size = sizeof(client_address);  /* Size del client */
    pid_t              process_id;                            /* ID of forked process */
    int                max_file_descriptor;                   /* Max socket to monitor */
    int                sockets_to_monitor[MAX_SOCKETS];       /* Array of sockets to monitor */
    fd_set             monitor_reader_socket;                 /* Sockets to monitor while reading */
    size_t             ready_sockets;                         /* Number of ready sockets */
    struct timeval     tm;                                    /* Time of activity session */
    size_t             n_read;                                /*  */

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

    /* Initialize all members of the array to -1 */
    memset(sockets_to_monitor, -1, sizeof(sockets_to_monitor));

    max_file_descriptor = listen_file_descriptor;
    sockets_to_monitor[max_file_descriptor] = 1;

    /*
     * ==================================
     * =            ENDLESS             =
     * ==================================
     * */
    for(;;)
    {
        int i;
        tm.tv_sec = 3 * 60;
        FD_ZERO(&monitor_reader_socket);

        /*
         * ==================================
         * =       INIT_SOCKETS_BITS        =
         * ==================================
         * */
        for(i = listen_file_descriptor; i <= max_file_descriptor; i++)
        {
            if(sockets_to_monitor[i] != -1)
            {
                FD_SET(i, &monitor_reader_socket);
            }
        }

        /*
         * =================================
         * =            SELECT             =
         * =================================
         * */
        if((ready_sockets = select(max_file_descriptor + 1, &monitor_reader_socket, NULL, NULL, &tm)) < 0)
        {
            fprintf(stderr, "Errore nella select\n");
            exit(EXIT_FAILURE);
        }
        else if(ready_sockets == 0)
        {
            //TODO da rivedere
            fprintf(stderr,"Sessione scaduta\n");
            break;
        }
        else
        {
            if(FD_ISSET(listen_file_descriptor, &monitor_reader_socket))
            {
                ready_sockets--;
                connection_file_descriptor = AcceptIPV4(listen_file_descriptor, &client_address, &client_size);

                PrintClientIPV4(&client_address, "Connected to");

                sockets_to_monitor[connection_file_descriptor] = 1;

                if(max_file_descriptor < connection_file_descriptor)
                {
                    max_file_descriptor = connection_file_descriptor;
                }
            }

            i = listen_file_descriptor;

            while(ready_sockets != 0)
            {
                i++;
                if(sockets_to_monitor[i] == -1)
                {
                    continue;
                }

                // TODO: Popolare con bzero();
                // TODO: Print delle informazioni client
                if(FD_ISSET(i, &monitor_reader_socket))
                {
                    ready_sockets--;
                    if((n_read = FullRead(i, reader_buffer, READER_BUFFER_SIZE)) > 0)
                    {
                        sockets_to_monitor[i] = -1;
                        if(max_file_descriptor == i)
                        {
                            while(sockets_to_monitor[--i] == -1);
                            max_file_descriptor = i;
                            break;
                        }
                    }
                    else
                    {
                        snprintf(writer_buffer, WRITER_BUFFER_SIZE, "%lu\n", strlen(reader_buffer) - 1);
                        FullWrite(i, writer_buffer, WRITER_BUFFER_SIZE);
                    }
                }
            }
        }
    }

    /* Irraggiungibile */
    exit(EXIT_SUCCESS);
}

