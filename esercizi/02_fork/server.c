#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "socket_utility.h"

#define BACKLOG 1024
#define BUFFER_SIZE 4096

int main() {
    int listen_file_descriptor;
    int connection_file_descriptor;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    char buffer[BUFFER_SIZE];

    socklen_t client_size = sizeof(client_address);
    time_t time_stamp;
    pid_t process_id;

    /* Creazione della socket */
    listen_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);

    /* Costruzione ENDPOINT del server */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(13);

    /* Collegamento del file descriptor all'indirizzo del server */
    BindIPV4(listen_file_descriptor, &server_address);

    /* Configuriamo il socket in modalità ascolto */
    Listen(listen_file_descriptor, BACKLOG);

    /* Gestione dei procesi zombie */
    signal(SIGCHLD,SIG_IGN);

    /* Accendiamo all'infinito il server */
    for(;;)
    {
        /* Chiamata bloccante per accettare richieste di connessione in coda */
        connection_file_descriptor = AcceptIPV4(listen_file_descriptor, &client_address, &client_size);

        /* Fork per gestire la connessione */
        if((process_id = fork()) < 0)
        {
            perror("Fork error: ");
            exit(-1);
        }

        /* Codice eseguibile solo dal processo figlio */
        if(process_id == 0)
        {
            /* Chiudiamo, solo sul processo figlio, il file descriptor configurato in modalità ascolto */
            close(listen_file_descriptor);
            /* Secondi passati dal 1 gennaio 1970 */
            time_stamp = time(NULL);
            /* Copiamo una stringa formattata nel buffer */
            snprintf(buffer, BUFFER_SIZE, "%.24s\n", ctime(&time_stamp));
            /* Eseguiamo la scrittura sul socket */
            FullWrite(connection_file_descriptor, buffer, strlen(buffer) + 1);

            printClientIPV4(&client_address,buffer,BUFFER_SIZE);

            /* Chiudiamo la connessione del client (solo processo figlio) */
            close(connection_file_descriptor);

            exit(0);
        }
        else
        {
            /* Processo padre*/
            close(connection_file_descriptor);
        }
    }


    /* Mai raggiunta */
    exit(0);
}