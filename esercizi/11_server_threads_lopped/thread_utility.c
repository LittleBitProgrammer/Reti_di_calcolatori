//
// Created by francesco on 10/10/22.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "thread_utility.h"
#include "socket_utility.h"

#define WRITER_BUFFER_SIZE 128
#define READER_BUFFER_SIZE 4096

void* server_handler(void* args_struct)
{
    char                writer_buffer[WRITER_BUFFER_SIZE];     /* Reader buffer */
    char                reader_buffer[READER_BUFFER_SIZE];     /* Writer buffer */
    char                ip_buffer[INET6_ADDRSTRLEN];
    arg_struct          arguments = *((arg_struct *) args_struct);

    int                 new_connection_file_descriptor = arguments.arg1;
    struct sockaddr_in  client_address = arguments.arg2;

    while(1)
    {
        if(FullRead(new_connection_file_descriptor, reader_buffer, READER_BUFFER_SIZE) > 0)
        {
            inet_ntop(AF_INET, &(client_address.sin_addr),ip_buffer,INET6_ADDRSTRLEN);
            fprintf(stderr, "Disconnected client: \nIP:PORT\t<%s:%d>\n", ip_buffer, ntohs(client_address.sin_port));

            close(new_connection_file_descriptor);
            pthread_exit(NULL);
        }
        else
        {
            PrintClientIPV4(&client_address, "Request from");
            snprintf(writer_buffer, WRITER_BUFFER_SIZE, "%lu\n", strlen(reader_buffer) - 1);

            if(FullWrite(new_connection_file_descriptor, writer_buffer, WRITER_BUFFER_SIZE) > 0)
            {
                fprintf(stderr, "Response Error\n");
            }
            else
            {
                PrintClientIPV4(&client_address, "Response to");
            }
        }
    }
}