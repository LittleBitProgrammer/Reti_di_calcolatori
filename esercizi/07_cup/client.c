#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "socket_utility.h"
#include "application_level_utility.h"

#define SIZE_LIST_LEN 2

int main(int argc, char **argv)
{
    /*
     * ==========================
     * =       Variables        =
     * ==========================
     * */
    int                socket_file_descriptor;              /* Socket client file descriptor */
    struct sockaddr_in server_address;                      /* Endpoint server */
    struct hostent*    server_dns;                          /* Structure dns server */
    Size_list*         size_list_buffer;                    /* Package that represents the size of exam's list */
    Request*           request_buffer;                      /* Package that represents the requested visit */
    Response*          response_buffer;                     /* Package that represents the response visit */
    short              menu_choice;                         /* Variable that store the client choice on menu */

    /*
     * ==========================
     * =       Arguments        =
     * ==========================
     * */
    if(argc != 2)
    {
        fprintf(stderr, "usage %s <IP/HOSTNAME>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * ==========================
     * =          DNS           =
     * ==========================
     * */
    if((server_dns = gethostbyname(argv[1])) == NULL)
    {
        herror("DNS client error");
        exit(EXIT_FAILURE);
    }

    /*
     * ==========================
     * =    SOCKET CREATION     =
     * ==========================
     * */
    socket_file_descriptor = Socket(server_dns->h_addrtype, SOCK_STREAM, 0);

    /*
     * ==========================
     * =    SERVER CREATION     =
     * ==========================
     * */
    server_address.sin_family = server_dns->h_addrtype;
    server_address.sin_port = htons(13);
    server_address.sin_addr = *((struct in_addr *)server_dns->h_addr_list[0]);

    /*
     * ==========================
     * =       CONNECTION       =
     * ==========================
     * */
    ConnectIPV4(socket_file_descriptor, &server_address);

    /*
     * ==========================
     * =     DOWNLOAD  MENU     =
     * ==========================
     * */
    FullRead(socket_file_descriptor, size_list_buffer, sizeof(*size_list_buffer));
    

    return 0;
}
