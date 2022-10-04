//
// Created by francesco on 04/10/22.
//
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>

#include "codice_fiscale.h"

#define COD_LEN 12 //20      /* Lunghezza massima della stringa */

/**
 * @brief Controlla se la lunghezza della stringa è giusta e se sono stati digitati solo numeri
 *
 * @param cod stringa da controllare
 *
 * @return ritorna TRUE se la stringa è giusta altrimenti ritorna FALSE
 * */
bool VerifyCod(char * cod){
    if (strlen(cod) < COD_LEN)               /* Caso in cui la lunghezza del codice sia piu corta */
    {
        printf("Lunghezza del codice troppo corta!\n");
        return FALSE;
    } else if (strlen(cod) > COD_LEN)        /* Caso in cui la lunghezza del codice sia piu lunga */
    {
        printf("Lunghezza del codice troppo lunga!\n");
        return FALSE;
    } else if (digits_only(cod))                /* Caso in cui la lunghezza sia giusta e controllo se sono stati digitati solo numeri*/
    {
        char * tmp = (char *) malloc(COD_LEN * sizeof(char));
        memcpy(tmp, cod, 5);
        if(!strcmp(tmp, "80380") && ControlloRegione(cod, tmp))               /* Controllo dei primi 5 numeri standard */
        {
            return TRUE;
        }

        free(tmp);
    }

    return FALSE;
}

/**
 * @brief Controllo se sono stati inseriti solo numeri
 *
 * @param cod codice da controllare
 *
 * @return TRUE se sono inseriti solo numeri altrimenti FALSE
 * */
bool digits_only(const char *cod)
{
    while (*cod)
    {
        if (isdigit(*cod++) == 0) return FALSE;
    }

    return TRUE;
}

/* Controllo sotto stringa del codice della regione  */
bool ControlloRegione(char* cod, char* tmp)
{
    strncpy(tmp, cod + 5, 5);
    char line[6];

    FILE *fp;
    fp = fopen("codiciRegioni", "r");

    while(fscanf(fp, "%s", line) == 1)
    {
        if(!strcmp(line, tmp))
        {
            return TRUE;
        }
    }
    fclose(fp);

    return FALSE;
}
