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
#define MAX_SOCKETS 256
#define BACKLOG 1024

#define LOG TRUE

int main()
{
    /* Rende STDOUT non bufferizzato */
    (void)setvbuf(stdout, NULL, _IONBF, 0);

    /*
     * ==========================
     * =       Variables        =
     * ==========================
     * */
    int                listen_file_descriptor;                /* Socket in listening */
    int                connection_file_descriptor;            /* Socket for the connection with client */
    int                temp_socket_file_descriptor;           /*  */
    int                is_address_reusable = 1;               /* Flag for socket option */
    struct sockaddr_in server_address;                        /* Structure for the server */
    struct sockaddr_in client_address;                        /* Structure for the client */
    char               writer_buffer[WRITER_BUFFER_SIZE];     /* Reader buffer */
    char               reader_buffer[READER_BUFFER_SIZE];     /* Writer buffer */
    socklen_t          client_size = sizeof(client_address);  /* Size del client */
    int                max_file_descriptor;                   /* Max socket to monitor */
    int                sockets_to_monitor[MAX_SOCKETS];       /* Array of sockets to monitor */
    fd_set             monitor_reader_socket;                 /* Sockets to monitor while reading */
    fd_set             copy_reader_socket;                    /*  */
    size_t             ready_sockets;                         /* Number of ready sockets */
    struct timeval     tm;                                    /* Time of activity session */
    int                max_index;                             /*  */
    char               ip_buffer[INET6_ADDRSTRLEN];           /*  */


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



    max_file_descriptor = listen_file_descriptor;
    max_index = -1;

    /* Initialize all members of the array to -1 */
    memset(sockets_to_monitor, -1, sizeof(sockets_to_monitor));

    FD_ZERO(&monitor_reader_socket);
    FD_SET(listen_file_descriptor, &monitor_reader_socket);

    tm.tv_sec = 3 * 60;
    tm.tv_usec = 0;

    /*
     * ==================================
     * =            ENDLESS             =
     * ==================================
     * */
    for(;;)
    {
        int i;
        copy_reader_socket = monitor_reader_socket;

        if((ready_sockets = select(max_file_descriptor + 1, &copy_reader_socket, NULL, NULL, &tm)) < 0)
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
            if(FD_ISSET(listen_file_descriptor, &copy_reader_socket))
            {
                connection_file_descriptor = AcceptIPV4(listen_file_descriptor, &client_address, &client_size);

                #ifdef LOG
                PrintClientIPV4(&client_address, "Connected to");
                #endif

                for(i = 0; i < MAX_SOCKETS; i++)
                {
                    if(sockets_to_monitor[i] < 0)
                    {
                        sockets_to_monitor[i] = connection_file_descriptor;
                        break;
                    }
                }
                if(i == MAX_SOCKETS)
                {
                    fprintf(stderr, "Troppi client\n");
                    close(i);
                    continue;
                }

                FD_SET(connection_file_descriptor, &monitor_reader_socket);

                if(connection_file_descriptor > max_file_descriptor)
                {
                    max_file_descriptor = connection_file_descriptor;
                }

                if(i > max_index)
                {
                    max_index = i;
                }

                if(--ready_sockets <= 0)
                {
                    continue;
                }
            }

            for(i = 0; i <= max_index; i++)
            {
                if((temp_socket_file_descriptor = sockets_to_monitor[i]) < 0)
                {
                    continue;
                }

                if(FD_ISSET(temp_socket_file_descriptor, &copy_reader_socket))
                {
                    if(FullRead(temp_socket_file_descriptor, reader_buffer, READER_BUFFER_SIZE) > 0)
                    {
                        /* Connection closed by client */
                        close(temp_socket_file_descriptor);

                        FD_CLR(temp_socket_file_descriptor, &monitor_reader_socket);
                        sockets_to_monitor[i] = -1;

                        inet_ntop(AF_INET, &(client_address.sin_addr),ip_buffer,INET6_ADDRSTRLEN);
                        fprintf(stderr, "Disconnected client: \nIP:PORT\t<%s:%d>\n", ip_buffer, ntohs(client_address.sin_port));
                    }
                    else
                    {
                        #ifdef LOG
                        PrintClientIPV4(&client_address, "Request from");
                        #endif
                        snprintf(writer_buffer, WRITER_BUFFER_SIZE, "%lu\n", strlen(reader_buffer) - 1);
                        if((FullWrite(temp_socket_file_descriptor, writer_buffer, WRITER_BUFFER_SIZE)) > 0)
                        {
                            fprintf(stderr, "Response Error\n");
                        }
                        else
                        {
                            #ifdef LOG
                            PrintClientIPV4(&client_address, "Response to");
                            #endif
                        }
                    }

                    if(--ready_sockets <= 0)
                    {
                        break;
                    }
                }
            }
        }
    }

    /* Irraggiungibile */
    exit(EXIT_SUCCESS);
}