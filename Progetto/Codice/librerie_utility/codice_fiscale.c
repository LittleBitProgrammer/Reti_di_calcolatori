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
bool InsertCode(char* code){
    /* Controllo se sono stati digitati solo numeri*/
    if(digits_only(code))
    {
        /* Caso in cui la lunghezza del codice sia piu corta */
        if (strlen(code) < CODE_LEN)
        {
            printf("Lunghezza del codice troppo corta!\n");
            return FALSE;
        }
        /* Caso in cui la lunghezza del codice sia piu lunga */
        else if (strlen(code) > CODE_LEN)
        {
            printf("Lunghezza del codice troppo lunga!\n");
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
            if(!strncmp(code, "80380", 5) && CheckRegion(code) && CheckCode(code))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**
 * @brief controllo dei 5 sottovalori della regione
 *
 * @param cod codice della tessera
 *
 * @return TRUE se il codice della regione è giusto altrimenti FALSE
 * */
bool CheckRegion(char* code)
{
    /* Dichiariamo il nome del file da aprire */
    char* file_name = "../codiciRegioni";
    /* Allochiamo spazio per il sottocodice corrispondente alla regione */
    char* sub_code = (char *) malloc(CODE_LEN * sizeof(char));

    /* Copiamo all'interno di "sub_code" i 5 numeri del codice che corrispondono alla regione */
    strncpy(sub_code, code + 5, 5);

    /* Apriamo il file contenente tutti i sottocodici delle regioni */
    FILE* fp;
    fp = fopen(file_name, "r");

    /* creazione di una variabile booleana per l'esito della funzione "StringCodeIsWritten" */
    bool controllo;

    /* Controlliamo se il file è stato aperto correttamente */
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file!\n");
        exit(EXIT_FAILURE);
    }
    /* Ritorno della funzione per il controllo se il codice esiste o meno */
    controllo = StringCodeIsWritten(fp, sub_code);

    /* Chiusura file */
    fclose(fp);

    /* Deallocazione della memoria */
    free(sub_code);

    return controllo;
}

/**
 * @brief controlla se il codice si trova in un file
 *
 * @param fp Puntatore al file
 * @param cod Codice da controllare
 *
 * @return TRUE se il codice si trova nel file altrimenti FALSE
 *
 * */
bool StringCodeIsWritten(FILE* fp, char* code)
{
    /* Buffer di lettura da file */
    char* buffer = (char *) malloc(CODE_LEN * sizeof(char));

    /* Lettura da file finchè non è EOF */
    while(fscanf(fp, "%s", buffer) != EOF)
    {
        /* Controllo della stringa se è uguale al buffer */
        if(!strcmp(buffer, code))
        {
            /* Deallocazione della memoria */
            free(buffer);
            return TRUE;
        }
    }
    /* Deallocazione della memoria */
    free(buffer);

    return FALSE;
}

/**
 * @brief Controlla se il codice è già stato inserito, se non è inserito lo inserisce
 *
 * @param code Codice da inserire
 *
 * @return TRUE se è stato inserito correttamente altrimenti FALSE
 * */
bool CheckCode(char* code)
{
    /* Puntatore a file */
    FILE* fp;

    char* file_name = "../codice_tessera_sanitaria";

    /* Apertura del file in lettura e scrittura */
    fp = fopen(file_name, "r+");

    /* Controllo se l'apertura del file è andata a buon fine */
    if (fp == NULL)
    {
        printf("Errore nell'apertura del file!\n");
        exit(EXIT_FAILURE);
    }

    /* Controllo del codice se esiste già o meno */
    if(!StringCodeIsWritten(fp, code))
    {
        /* Inserimento del codice nel file */
        fprintf(fp, "%s\n", code);
        printf("Codice inserito correttamente\n");

        /* Chiusura file */
        fclose(fp);
        return TRUE;
    }

    /* Stampa nel caso il codice già esista */
    printf("Codice già esistente!\n");

    /* Chiusura file */
    fclose(fp);

    return FALSE;
}
