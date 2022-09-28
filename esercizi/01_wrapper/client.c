#include <stdio.h>
#include <stdlib.h>
#include "socket_utility.h"

int main(int argc, char **argv)
{
    int socket_file_descriptor;
    struct sockaddr_in server_address;

    char receive_line[1025];

    if(argc != 2)
    {
        fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }

    socket_file_descriptor = Socket(AF_INET,SOCK_STREAM,0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(13);
    if (inet_pton(AF_INET,argv[1],&server_address.sin_addr) <= 0)
    {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(1);
    }

    ConnectIPV4(socket_file_descriptor, &server_address);
    FullRead(socket_file_descriptor, receive_line, 1025);
    if (fputs(receive_line,stdout) == EOF)
    {
        fprintf(stderr,"fputs error\n");
        exit(1);
    }

    exit(0);
}

