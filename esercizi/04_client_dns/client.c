/**
 * Nome: client.c
 * Scopo: Mostrare l'utilizzo della risoluzione DNS diretta e inversa, sfruttando il resolver C
 */
#include <stdio.h>
#include <netdb.h> /* Definizione di hostent, herror() */
#include <stdlib.h> /* Funzione exit */
#include <arpa/inet.h> /* INET6_ADDRSTRLEN */

/* Permettiamo al programma eseguibile di accettare argomenti in input */
int main(int argc, char **argv) {

    struct hostent *direct_dns; /* Indirizzo risoluzione diretta */
    struct hostent *reverse_dns; /* Indirizzo risoluzione inversa */

    struct in_addr reverse_address; /* Indirizzo formato network order */

    char **alias; /* Array di stringhe per immagazzinare alias */
    char **ip_addresses; /* Array di stringhe per immagazzinare indirizzi IP */
    char *temp_address; /* Indirizzo corrente (un dominio può avere più IP) */
    char buffer[INET6_ADDRSTRLEN]; /* Array per memorizzare un indirizzo IP con grandezza massima di un IPV6 */

    /* Controlliamo che l'argomento passato in input, oltre al comando, sia uno solo */
    if(argc < 2)
    {
        printf("Usage: %s <nomehost>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * ===========================================
     * =           Risoluzione Diretta           =
     * ===========================================
     */

    printf("Risoluzione diretta\n\n");

    /* Popoliamo la struttura direct_dns attraverso la funzione gethostbyname */
    direct_dns = gethostbyname(argv[1]);

    /* Controlliamo che la struttura popolata non sia nulla */
    if(direct_dns == NULL)
    {
        herror("Errore di risoluzione Diretta");
        exit(EXIT_FAILURE);
    }

    /* Stampa del nome canonico */
    printf("Canonical name %s\n", direct_dns->h_name);

    /* Stampa degli alias */
    alias = direct_dns->h_aliases;
    while(*alias != NULL)
    {
        printf("Alias %s\n", *alias);
        alias++;
    }

    switch (direct_dns->h_addrtype) {
        case AF_INET:
            printf("Addresses are of type IPV4\n");
            break;
        case AF_INET6:
            printf("Addresses are of type IPV6\n");
            break;
        default:
            printf("Wrong Addresses type\n");
            exit(EXIT_FAILURE);
    }

    /* Stampa degli indirizzi IP */
    ip_addresses = direct_dns->h_addr_list;
    while(*ip_addresses != NULL)
    {
        temp_address = (char *) inet_ntop(direct_dns->h_addrtype, *ip_addresses, buffer, INET6_ADDRSTRLEN);
        printf("Indirizzo %s\n", temp_address);
        ip_addresses++;
    }

    /*
     * ===========================================
     * =           Risoluzione Inversa           =
     * ===========================================
     */

    printf("\nRisoluzione inversa\n\n");

    /* Stampa indirizzo IP */
    printf("Indirizzo ip: %s\n", temp_address);

    /* Trasformiamo l'indirizzo da formato stringa a network order e lo immagazziniamo nella variabile reverse_address */
    inet_aton(temp_address, &reverse_address);

    /* Popoliamo la struttura direct_dns attraverso la funzione gethostbyaddr */
    reverse_dns = gethostbyaddr((const char *) &reverse_address, sizeof(reverse_address), direct_dns->h_addrtype);

    /* Controlliamo che la struttura popolata non sia nulla */
    if(reverse_dns == NULL)
    {
        herror("Errore di risoluzione Inversa");
        exit(EXIT_FAILURE);
    }

    /* Stampiamo gli alias */
    alias = reverse_dns->h_aliases;
    while(*alias != NULL)
    {
        printf("alias %s\n", *alias);
        alias++;
    }

    /* Stampiamo il nome canonico */
    printf("Canonical name %s\n", reverse_dns->h_name);

    exit(EXIT_SUCCESS);
}
