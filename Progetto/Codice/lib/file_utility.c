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
File_result is_code_written_in_file(char *file_name, char *code)
{
    File_result file_errors;

    /* Buffer di lettura da file */
    char *line = (char *)malloc(56 * sizeof(char));
    char *tokens;

    /* Apriamo il file contenente tutti i sottocodici delle regioni */
    FILE* file_codes;

    /* Controlliamo se il file è stato aperto correttamente */
    if ((file_codes = fopen(file_name, "r")) == NULL)
    {
        file_errors.result_flag = 0;
        file_errors.file_flags.open_file_flag = 1;
        file_errors.file_flags.write_file_flag = 0;

        fprintf(stderr, "2. Errore nell'apertura del file!\n");
        return file_errors;
    }

    /* Lettura da file finchè non è EOF */
    while (!feof(file_codes))
    {
        fgets(line, 56, file_codes);
        tokens = strtok(line, " ");

        if(tokens == NULL)
        {
            file_errors.result_flag = 0;
            file_errors.file_flags.open_file_flag = 0;
            file_errors.file_flags.write_file_flag = 0;

            /* Deallocazione della memoria */
            free(tokens);
            fclose(file_codes);
            return file_errors;
        }
        /*
         * Cerca la prima occorrenza della seconda stringa nella prima stringa e restituisce l'indice. Così è possibile cambiargli
         * il valore a '\0'
         * */
        tokens[strcspn(tokens, "\n")] = '\0';
        if (!strcmp(tokens, code))
        {
            file_errors.result_flag = 1;
            file_errors.file_flags.open_file_flag = 0;
            file_errors.file_flags.write_file_flag = 0;

            /* Deallocazione della memoria */
            free(tokens);
            fclose(file_codes);
            return file_errors;
        }
    }

    file_errors.result_flag = 0;
    file_errors.file_flags.open_file_flag = 0;
    file_errors.file_flags.write_file_flag = 0;

    /* Deallocazione della memoria */
    free(line);

    /* Chiusura del file */
    fclose(file_codes);

    return file_errors;
}

/**
 * @brief
 *
 * @param vaccinated_client_info
 *
 * @return
 * */
File_result subscribe_vaccinated_client(char* vaccinated_client_info)
{
    File_result file_errors;
    FILE *vaccinated_file;

    if((vaccinated_file = fopen(VACCINATED_FILE_NAME, "a")) == NULL)
    {
        file_errors.result_flag = 0;
        file_errors.file_flags.open_file_flag = 1;
        file_errors.file_flags.write_file_flag = 0;

        fprintf(stderr, "1. Errore nell'apertura del file!\n");
        return file_errors;
    }

    if(fprintf(vaccinated_file, "%s\n", vaccinated_client_info) < 0)
    {
        file_errors.result_flag = 0;
        file_errors.file_flags.open_file_flag = 0;
        file_errors.file_flags.write_file_flag = 1;

        fprintf(stderr, "Errore in scrittura su File\n");
        fclose(vaccinated_file);
        return file_errors;
    }

    file_errors.result_flag = 1;
    file_errors.file_flags.open_file_flag = 0;
    file_errors.file_flags.write_file_flag = 0;

    fclose(vaccinated_file);
    return file_errors;
}

