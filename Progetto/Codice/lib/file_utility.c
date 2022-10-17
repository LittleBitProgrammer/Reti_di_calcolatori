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

int count_file_lines(char* file_name, char** list_codes)
{
    FILE* file_point;
    int line_counter;
    int read_character;
    char buffer[21];
    int counter = 0;

    if((file_point = fopen(file_name, "r")) == NULL)
    {
        fprintf(stderr, "Errore in apertura del file");
        return -1;
    }

    while((read_character = fgetc(file_point)) != EOF)
    {
        if(read_character == '\n')
        {
            line_counter++;
        }
    }

    if(line_counter != 0)
    {
        *list_codes = (char*)malloc(line_counter * 21);
        line_counter = 0;

        fseek(file_point, 0, SEEK_SET);
        while(fscanf(file_point, "%s", buffer) != EOF)
        {
            if(counter % 4 == 0)
            {
                //list_codes[line_counter] = (char*)malloc(21);
                strcpy((*list_codes + (line_counter * 21)), buffer);
                line_counter++;
            }

            counter++;
        }
    }

    fclose(file_point);
    return line_counter;
}

bool change_information_in_file(Administrator_request_package* administrator_package, Administrator_response_package* administrator_response, char* file_name)
{
    FILE* file_point;
    int count_lines = 0;
    long position_cursor;
    char* overwrite_string = (char*) malloc(MAX_FILE_LINE_SIZE);
    time_t daytime;
    struct tm* local_time;

    if((file_point = fopen(file_name, "r+")) == NULL)
    {
        fprintf(stderr, "Errore in apertura del file");
        administrator_response->reviser_package.file_flags.open_file_flag = 1;
        administrator_response->reviser_package.file_flags.write_file_flag = 0;
        administrator_response->reviser_package.file_flags.read_file_flag = 0;
        return FALSE;
    }

    daytime = time(NULL);
    local_time = localtime(&daytime);

    if (strlen(overwrite_string) < MAX_FILE_LINE_SIZE - 1)
    {
        memset(overwrite_string, 32, MAX_FILE_LINE_SIZE - 1);
    }

    while(count_lines != administrator_package->index_list)
    {
        if((fgetc(file_point)) == '\n')
        {
            count_lines++;
        }
    }

    // Store the position
    position_cursor = ftell(file_point);

    fscanf(file_point, "%s %d/%d/%d", administrator_response->code, &(administrator_response->reviser_package.expiration_date.tm_mday),
                                                       &(administrator_response->reviser_package.expiration_date.tm_mon),
                                                       &(administrator_response->reviser_package.expiration_date.tm_year));

    //Costruzione della stringa da inserire
    snprintf(overwrite_string, MAX_FILE_LINE_SIZE - 1, "%s %d/%d/%d %s %d/%d/%d", administrator_response->code,
                                                                                                   administrator_response->reviser_package.expiration_date.tm_mday,
                                                                                                   administrator_response->reviser_package.expiration_date.tm_mon,
                                                                                                   administrator_response->reviser_package.expiration_date.tm_year,
                                                                                                   administrator_package->motivation,
                                                                                                   local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
    overwrite_string[strlen(overwrite_string)] = 32;

    // Or fseek(file,ftell(file),SEEK_SET);
    fseek(file_point,position_cursor,SEEK_SET);

    //stampo la nuova stringa
    fprintf(file_point, "%s\n", overwrite_string);

    fclose(file_point);
    return TRUE;
}


