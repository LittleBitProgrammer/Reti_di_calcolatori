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
     * =             ENDLESS            =
     * ==================================
     * */
    for(;;)
    {

    }

    /* Irraggiungibile */
    exit(EXIT_SUCCESS);
}
