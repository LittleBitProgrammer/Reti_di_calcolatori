#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "codice_fiscale.h"

#define CODE_LEN 20      /* Lunghezza massima della stringa */

/**
 * @brief Controlla se la lunghezza della stringa è giusta e se sono stati digitati solo numeri
 *
 * @param code stringa da controllare
 *
 * @return ritorna TRUE se la stringa è giusta altrimenti ritorna FALSE
 * */
bool insertCode(char* code){
    /* Controllo se sono stati digitati solo numeri*/
    if(digits_only(code))
    {
        /* Caso in cui la lunghezza del codice sia piu corta */
        if (strlen(code) < CODE_LEN)
        {
            fprintf(stderr,"Lunghezza del codice troppo corta!\n");
            return FALSE;
        }
        /* Caso in cui la lunghezza del codice sia piu lunga */
        else if (strlen(code) > CODE_LEN)
        {
            fprintf(stderr,"Lunghezza del codice troppo lunga!\n");
            return FALSE;
        }
        /* Caso in cui la lughezza sia giusta*/
        else
        {
            /*
             * Controllo su:
             *      1. I primi 5 numeri standard per ogni codice;
             *      2. I successivi 5 numeri che corrispondono alle regioni italiane;
             *      3. Sul successo dell'inserimento all'intero del codice nel file.
             * */
            if(!strncmp(code, "80380", 5) && checkRegion(code))
            {
                return TRUE;
            }
        }
    }

    fprintf(stderr, "Non sono solo numeri!\n");
    return FALSE;
}

/**
 * @brief controllo dei 5 sottovalori della regione
 *
 * @param code codice della tessera
 *
 * @return TRUE se il codice della regione è giusto altrimenti FALSE
 * */
bool checkRegion(char* code)
{
    /* Dichiariamo il nome del file da aprire */
    char* file_name = "codiciRegioni";
    /* Allochiamo spazio per il sottocodice corrispondente alla regione */
    char sub_code[6];

    /* Copiamo all'interno di "sub_code" i 5 numeri del codice che corrispondono alla regione */
    strncpy(sub_code, code + 5, 5);
    *(sub_code + 5) = '\0';

    /* Ritorno della funzione per il controllo se il codice esiste o meno */
    return stringCodeIsWritten(file_name, sub_code);
}

/**
 * @brief controlla se il codice si trova in un file
 *
 * @param file_name nome del file da aprire
 * @param code Codice da controllare
 *
 * @return TRUE se il codice si trova nel file altrimenti FALSE
 * */
bool isWrittenInFile(char * file_name, char* code)
{
    /* Buffer di lettura da file */
    char* buffer = (char *) malloc(6 * sizeof(char));

    /* Apriamo il file contenente tutti i sottocodici delle regioni */
    FILE* fp;

    /* Controlliamo se il file è stato aperto correttamente */
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        fprintf(stderr, "Errore nell'apertura del file!\n");
        exit(EXIT_FAILURE);
    }

    /* Lettura da file finchè non è EOF */
    while(fscanf(fp, "%s", buffer) != EOF)
    {
        /* Controllo della stringa se è uguale al buffer */
        if(!strcmp(buffer, code))
        {
            /* Chiusura del file */
            fclose(fp);

            /* Deallocazione della memoria */
            free(buffer);

            return TRUE;
        }
    }
    /* Deallocazione della memoria */
    free(buffer);

    /* Chiusura del file */
    fclose(fp);

    return FALSE;
}

