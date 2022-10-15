#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    /* ==========================
     * =       Variables        =
     * ==========================
     */

    int client_file_descriptor;
    struct sockaddr_in server_address;
    struct hostent* server_dns;

    /*
    * ==========================
    * =       Arguments        =
    * ==========================
    * */

    /*
     * Attraverso il seguente costrutto di controllo si va a controllare che sia stato inserito un numero di argomenti pari ad 1,
     * in quanto anche il comando digitato su linea di comando viene contato come argomento
     * */
    if(argc != 2)
    {
        fprintf(stderr, "Client usage: %s <IP ADDRESS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    



    return 0;
}