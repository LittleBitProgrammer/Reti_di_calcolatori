#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "socket_utility.h"

#define BUFF_SIZE 4096
#define BACKLOG 1024

int main()
{
    int listen_file_descriptor;
    int connection_file_descriptor;

    char write_buffer[BUFF_SIZE];
    time_t ticks;

    struct sockaddr_in server_address;

    listen_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(13);

    BindIPV4(listen_file_descriptor, &server_address);
    Listen(listen_file_descriptor, BACKLOG);

    for(;;)
    {
        connection_file_descriptor = Accept(listen_file_descriptor,NULL,NULL);

        ticks = time(NULL);
        snprintf(write_buffer, sizeof(write_buffer), "%.24s\n", ctime(&ticks));

        if(FullWrite(connection_file_descriptor, write_buffer, strlen(write_buffer) + 1) < 0)
        {
            perror("Write: ");
            exit(1);
        }

        close(connection_file_descriptor);
    }
}
