#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utility.h"

/**
 * @brief
 *
 * @param file_name
 * @param code
 *
 * @return
 * */
//TODO: Cambiare nomi di tutte le variabili
bool is_code_written_in_file(char *file_name, char *code)
{
    /* Buffer di lettura da file */
    char *line = (char *)malloc(56 * sizeof(char));
    char *tokens;

    /* Apriamo il file contenente tutti i sottocodici delle regioni */
    FILE* file_codes;

    /* Controlliamo se il file è stato aperto correttamente */
    if ((file_codes = fopen(file_name, "r")) == NULL)
    {
        fprintf(stderr, "Errore nell'apertura del file!\n");
        exit(EXIT_FAILURE);
    }

    /* Lettura da file finchè non è EOF */
    while (!feof(file_codes))
    {
        fgets(line, 56, file_codes);
        tokens = strtok(line, " ");
        if (!strcmp(tokens, code))
        {
            /* Deallocazione della memoria */
            free(tokens);
            fclose(file_codes);
            return TRUE;
        }
    }
    /* Deallocazione della memoria */
    free(line);

    /* Chiusura del file */
    fclose(file_codes);

    return FALSE;
}

