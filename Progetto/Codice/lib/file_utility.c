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
        return FALSE;
    }

    /* Lettura da file finchè non è EOF */
    while (!feof(file_codes))
    {
        fgets(line, 56, file_codes);
        tokens = strtok(line, " ");

        if(tokens == NULL)
        {
            /* Deallocazione della memoria */
            free(tokens);
            fclose(file_codes);
            return FALSE;
        }
        /*
         * Cerca la prima occorrenza della seconda stringa nella prima stringa e restituisce l'indice. Così è possibile cambiargli
         * il valore a '\0'
         * */
        tokens[strcspn(tokens, "\n")] = '\0';
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

/**
 * @brief
 *
 * @param vaccinated_client_info
 *
 * @return
 * */
 //TODO: struct bool degli errori
bool subscribe_vaccinated_client(char* vaccinated_client_info)
{
    FILE *vaccinated_file;

    if((vaccinated_file = fopen(VACCINATED_FILE_NAME, "a")) == NULL)
    {
        fprintf(stderr, "Errore nell'apertura del file!\n");
        return FALSE;
    }

    if(fprintf(vaccinated_file, "%s\n", vaccinated_client_info) < 0)
    {
        fprintf(stderr, "Errore in scrittura su File\n");
        fclose(vaccinated_file);
        return FALSE;
    }

    fclose(vaccinated_file);
    return TRUE;
}

