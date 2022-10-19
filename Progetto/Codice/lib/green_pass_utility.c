#include <stdio.h>
#include <string.h>
#include "green_pass_utility.h"
#include "file_utility.h"
#include "date_utility.h"

File_flags* generate_reviser_response(Reviser_package* reviser_package, char* card_code)
{
    FILE* vaccinated_file;
    char temp_card_code[21];
    struct tm* daytime;
    time_t time_stamp = time(NULL);

    if((vaccinated_file = fopen(VACCINATED_FILE_NAME, "r")) == NULL)
    {
        reviser_package->file_flags.open_file_flag = 1;
        reviser_package->file_flags.write_file_flag = 0;

        fprintf(stderr, "3.Errore nell'apertura del file!\n");
        return &(reviser_package->file_flags);
    }

    while(fscanf(vaccinated_file, "%s %d/%d/%d %s %d/%d/%d\n", temp_card_code, &(reviser_package->expiration_date.tm_mday),
                                                                           &(reviser_package->expiration_date.tm_mon), &(reviser_package->expiration_date.tm_year),
                                                                           reviser_package->motivation, &(reviser_package->last_update.tm_mday),
                                                                           &(reviser_package->last_update.tm_mon), &(reviser_package->last_update.tm_year)) != EOF)
    {
        if(!strcmp(temp_card_code, card_code))
        {
            reviser_package->expiration_date.tm_mon -= 1;
            reviser_package->expiration_date.tm_year -= 1900;
            reviser_package->last_update.tm_mon -= 1;
            reviser_package->last_update.tm_year -= 1900;

            /* Linea trovata */
            break;
        }

        bzero(reviser_package, sizeof(*reviser_package));
    }

    if((daytime = localtime(&time_stamp)) == NULL)
    {
        return NULL;
    }
    reviser_package->is_green_pass_valid = check_green_pass_validity(&reviser_package->expiration_date, daytime, reviser_package->motivation);
    fclose(vaccinated_file);
    return NULL;
}

bool check_green_pass_validity(struct tm* expiration_date, struct tm* today_date, char* motivation)
{
    if(!strcmp(motivation, "Covid"))
    {
        return FALSE;
    }

    if(compare_date(today_date, expiration_date) < 0)
    {
        strcpy(motivation, "Scaduto");
        return FALSE;
    }

    return TRUE;
}
