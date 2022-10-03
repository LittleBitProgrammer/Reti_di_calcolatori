#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "socket_utility.h"
#include "application_level_utility.h"

#define BACKLOG 1024
#define EXAM_COLUMNS 255

int main(int argc, char** argv)
{
    /*
     * ==========================
     * =       VARIABLES        =
     * ==========================
     * */
    int                listen_file_descriptor;                                          /* Socket in listening mode */
    int                connection_file_descriptor;                                      /* Socket for the connection with the client */
    int                is_address_reusable = 1;                                         /* Flag for socket option */
    struct sockaddr_in server_address;                                                  /* Sever's structure */
    struct sockaddr_in client_address;                                                  /* Client's structure */
    Request*           request_buffer = (Request *)malloc(sizeof(Request));        /* Package that represents the requested visit */
    Response*          response_buffer = (Response *)malloc(sizeof(Response));     /* Package that represents the response visit */
    socklen_t          client_size = sizeof(client_address);                            /* Client's size */
    pid_t              process_id;                                                      /* ID of forked process */
    time_t             ticks;                                                           /* Seconds from 01/01/1970 until now */
    char               ip_buffer[INET6_ADDRSTRLEN];                                     /* Buffer of IP address dotted decimal */
    FILE*              exams_file;                                                      /* File with the exams */
    const char*        exams_file_path = "exams";                                       /* File path */
    Size_list*         size_list_buffer = (Size_list *)malloc(sizeof(Size_list));  /* Package that represents the size of exam's list */

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
     * =    SOCKET CREATION     =
     * ==========================
     * */
    listen_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    /*
     * ==========================
     * =    SERVER CREATION     =
     * ==========================
     * */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(13);

    /*
     * ==========================
     * =    SOCKET  OPTIONS     =
     * ==========================
     * */
    setsockopt(listen_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &is_address_reusable, sizeof(is_address_reusable));

    /*
     * ==========================
     * =          BIND          =
     * ==========================
     * */
    BindIPV4(listen_file_descriptor, &server_address);

    /*
     * ==========================
     * =         LISTEN         =
     * ==========================
     * */
    Listen(listen_file_descriptor, BACKLOG);

    /*
     * ==========================
     * =         ZOMBIE         =
     * ==========================
     * */
    signal(SIGCHLD, SIG_IGN);

    /*
     * ==========================
     * =      ENDLESS LOOP      =
     * ==========================
     * */
    for(;;)
    {
        /*
         * ==========================
         * =         ACCEPT         =
         * ==========================
         * */
        connection_file_descriptor = AcceptIPV4(listen_file_descriptor, &client_address, &client_size);

        /*
         * ==========================
         * =         FORK           =
         * ==========================
         * */
        if((process_id = fork()) < 0)
        {
            perror("Fork error: ");
            exit(EXIT_FAILURE);
        }

        /*
         * ==========================
         * =          CHILD         =
         * ==========================
         * */
        if(process_id == 0)
        {
            close(listen_file_descriptor);

            /*
             * ==========================
             * =      PRINT CLIENT      =
             * ==========================
             * */
            PrintClientIPV4(&client_address, ip_buffer, INET6_ADDRSTRLEN);

            /*
             * ==========================
             * =      WRITE  EXAMS      =
             * ==========================
             * */
            /* Open a read-only file */
            if((exams_file = fopen(exams_file_path, "r")) == NULL)
            {
                perror("File open Error");
                exit(EXIT_FAILURE);
            }

            /* Reading the first integer in the file */
            fscanf(exams_file, "%d", &(size_list_buffer->rows));
            size_list_buffer->columns = EXAM_COLUMNS;

            FullWrite(connection_file_descriptor, size_list_buffer, sizeof(*size_list_buffer));

            /* Reading all the exams */
            char exam_list[size_list_buffer->rows][size_list_buffer->columns];
            int i = 0;

            while(fscanf(exams_file, "%s", exam_list[i++]) == 1);

            FullWrite(connection_file_descriptor, exam_list, sizeof(exam_list));


            close(connection_file_descriptor);
            exit(EXIT_SUCCESS);
        }
        else /* Parent */
        {
            close(connection_file_descriptor);
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

    /* Unreachable */
    exit(EXIT_SUCCESS);
}


