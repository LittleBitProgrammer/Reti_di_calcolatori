//
// Created by francesco on 10/10/22.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "thread_utility.h"
#include "socket_utility.h"

#define WRITER_BUFFER_SIZE 128
#define READER_BUFFER_SIZE 4096

void* server_handler(void* connection_file_descriptor)
{
    char                writer_buffer[WRITER_BUFFER_SIZE];     /* Reader buffer */
    char                reader_buffer[READER_BUFFER_SIZE];     /* Writer buffer */

    int                 new_connection_file_descriptor = *((int *) connection_file_descriptor);

    if(FullRead(new_connection_file_descriptor, reader_buffer, READER_BUFFER_SIZE) > 0)
    {
        fprintf(stderr, "Client is disconnected\n");
        close(new_connection_file_descriptor);
        exit(EXIT_FAILURE);
    }
    else
    {
        snprintf(writer_buffer, WRITER_BUFFER_SIZE, "%lu\n", strlen(reader_buffer) - 1);

        if(FullWrite(new_connection_file_descriptor, writer_buffer, WRITER_BUFFER_SIZE) > 0)
        {
            fprintf(stderr, "Response Error\n");
        }
        else
        {
            close(new_connection_file_descriptor);
            pthread_exit(NULL);
        }
    }
}