#include <stdio.h>
#include <string.h>
#include "digits_utility.h"
#include "code_verification.h"
#include "file_utility.h"

/**
 * @brief
 *
 * @param code
 *
 * @return
 * */
bool insert_card_code(char* code)
{
    size_t code_len = strlen(code);

    /* Controllo se sono stati digitati solo numeri*/
    if(is_digits_only(code))
    {
        /* Caso in cui la lunghezza del codice sia piu corta */
        if (code_len < (MAX_CODE_LEN - 1) || code_len > (MAX_CODE_LEN - 1))
        {
            /* Caso in cui la lunghezza del codice sia piu lunga o più corta*/
            fprintf(stderr,"Lunghezza codice errata\n");
            return FALSE;
        }
        /* Caso in cui la lughezza sia giusta*/
        else
        {
            /*
             * Controllo su:
             *      1. I primi 5 numeri standard per ogni codice;
             *      2. I successivi 5 numeri che corrispondono alle regioni italiane;
             * */
            if(!strncmp(code, "80380", 5) && check_region_code(code))
            {
                return TRUE;
            }
            else
            {
                fprintf(stderr, "Codice di verifica non valido\n");
                return FALSE;
            }
        }
    }

    fprintf(stderr, "Il codice inserito non è composto da soli numeri\n");
    return FALSE;
}

/**
 * @brief
 *
 * @param code
 *
 * @return
 * */
bool check_region_code(char *code)
{
    /* */
    char sub_code[6];

    /* Copiamo all'interno di "sub_code" i 5 numeri del codice che corrispondono alla regione */
    strncpy(sub_code, code + 5, 5);
    *(sub_code + 5) = '\0';

    /* Ritorno della funzione per il controllo se il codice esiste o meno */
    return is_code_written_in_file(REGION_FILE_NAME, sub_code).result_flag;
}