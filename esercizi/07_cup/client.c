#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "socket_utility.h"
#include "application_level_utility.h"
#include "menu_utility.h"

int main(int argc, char **argv)
{
    /*
     * ==========================
     * =       VARIABLES        =
     * ==========================
     * */
    int                socket_file_descriptor;                                         /* Socket client file descriptor */
    struct sockaddr_in server_address;                                                 /* Endpoint server */
    struct hostent*    server_dns;                                                     /* Structure dns server */
    Size_list*         size_list_buffer = (Size_list *)malloc(sizeof(Size_list)); /* Package that represents the size of exam's list */
    Request*           request_buffer = (Request *)malloc(sizeof(Request));       /* Package that represents the requested visit */
    Response*          response_buffer = (Response *)malloc(sizeof(Response));    /* Package that represents the response visit */
    char*              command_buffer;                                                 /* Variable to decide the action of the server */
    size_t             len_command_buffer;                                             /* Command buffer size */
    short              menu_choice;                                                    /* Variable that store the client choice on menu */
    /*
     * ==========================
     * =    VARIABLE HANDLER    =
     * ==========================
     * */
    if(size_list_buffer == NULL || response_buffer == NULL || response_buffer == NULL)
    {
        perror("Error while allocating memory: ");
        exit(EXIT_FAILURE);
    }

    /*
     * ==========================
     * =       ARGUMENTS        =
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
    socket_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

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
    char exam_list[size_list_buffer->rows][size_list_buffer->columns];

    printf("exam_list: %lu\n", sizeof(exam_list));

    FullRead(socket_file_descriptor, exam_list, sizeof(exam_list));

    while(1)
    {
        /*
         * ==========================
         * =      PRINT  MENU       =
         * ==========================
         * */
        print_menu();
        scanf("%hd", &menu_choice);

        fflush(stdin);

        /*
         * ==========================
         * =      MENU CHOICES      =
         * ==========================
         * */
        switch (menu_choice) {
            case 1:
                command_buffer = "CMD_EXAM";
                len_command_buffer = strlen(command_buffer) + 1;

                /* Write della lunghezza */
                FullWrite(socket_file_descriptor, &len_command_buffer, sizeof(len_command_buffer));
                /* Write del buffer di comandi */
                FullWrite(socket_file_descriptor, command_buffer, len_command_buffer);

                print_menu_voices(size_list_buffer->rows, size_list_buffer->columns, exam_list);
                printf("\nInserire esame da prenotare: ");

                scanf("%hd", &menu_choice);
                fflush(stdin);

                booking_request(request_buffer);

                FullWrite(socket_file_descriptor, request_buffer, sizeof(*request_buffer));
                break;

                /*
            case 2:
                break;
                */

            case 3:
                command_buffer = "CMD_EXIT";
                len_command_buffer = strlen(command_buffer) + 1;
                FullWrite(socket_file_descriptor, &len_command_buffer, sizeof(len_command_buffer));
                FullWrite(socket_file_descriptor, command_buffer, len_command_buffer);

                printf("Arrivederci");

                free(size_list_buffer);
                free(request_buffer);
                free(response_buffer);
                exit(EXIT_SUCCESS);


            default:
                fprintf(stderr, "Il comando selezionato non è valido\n");

                free(size_list_buffer);
                free(request_buffer);
                free(response_buffer);
                exit(EXIT_FAILURE);
        }
    }


    /*
     * ==========================
     * =       FREE HEAP        =
     * ==========================
     * */
    free(size_list_buffer);
    free(request_buffer);
    free(response_buffer);
    return 0;
}
