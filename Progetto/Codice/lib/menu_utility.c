#include <stdio.h>              /*  */
#include <string.h>
#include "menu_utility.h"       /*  */
#include "date_utility.h"       /*  */
#include "code_verification.h"
#include "buffer_utility.h"     /*  */

/**
 * @brief
 *
 * @param vaccination_date
 * @param  local_daytime
 *
 * @return
 * */
bool run_vaccinated_menu(struct tm* vaccination_date, struct tm* local_daytime, char* verification_code)
{
    print_logo();
    printf("Benvenuti nel centro vaccinale.\n\n");
    printf("Inserire data di vaccinazione nel formato DD/MM/YYYY: ");

    if(!input_date(vaccination_date, local_daytime))
    {
        return FALSE;
    }

    printf("Inserire codice tessera sanitaria (controllare il punto '8' sul retro della tessera)\n> ");

    free_input_buffer();

    fscanf(stdin, "%s", verification_code);

    if(!verify_card_code(verification_code))
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief
 *
 * @param card_code
 *
 * @return
 * */
bool run_reviser_menu(char *card_code)
{
    print_logo();
    printf("Benvenuti sulla piattaforma Green Pass.\n\n");
    printf("Inserire codice tessera sanitaria (controllare il punto '8' sul retro della tessera)"
                  "di cui si vuole conoscere la scadenza Green Pass: ");
    fscanf(stdin,"%s", card_code);

    free_input_buffer();

    if(!verify_card_code(card_code))
    {
        return FALSE;
    }

    return TRUE;
}

bool run_administrator_menu(char* card_code, char* motivation)
{
    print_logo();
    printf("Benvenuti sulla piattaforma Green Pass.\n\n");
    printf("Inserire codice tessera sanitaria (controllare il punto '8' sul retro della tessera)"
                  "di cui si vuole aggiornare lo stato: ");

    fscanf(stdin,"%s", card_code);

    free_input_buffer();

    if(!verify_card_code(card_code))
    {
        return FALSE;
    }



    return TRUE;
}


/**
 * @brief
 * */
void print_logo(void)
{
    printf("   ______    ___   ____   ____  _____  ______      __    ______ \n");
    printf(" .' ___  | .'   `.|_  _| |_  _||_   _||_   _ `.   /  | .' ____ '.\n");
    printf("/ .'   \\_|/  .-.  \\ \\ \\   / /    | |    | | `. \\  `| | | (____) |\n");
    printf("| |       | |   | |  \\ \\ / /     | |    | |  | |   | | '_.____. |\n");
    printf("\\ `.___.'\\\\  `-'  /   \\ ' /     _| |_  _| |_.' /  _| |_| \\____| |\n");
    printf(" `.____ .' `.___.'     \\_/     |_____||______.'  |_____|\\______,' \n\n");
}

void print_reviser_result(Reviser_package* reviser_response, int divider_length)
{
    print_menu_divider(divider_length);
    printf("= Validità:             %s %s\n",    "Green Pass", reviser_response->is_green_pass_valid ? "valido" : "non valido");
    print_menu_divider(divider_length);
    printf("= Scadenza:             %d/%d/%d\n", reviser_response->expiration_date.tm_mday,
                                                          reviser_response->expiration_date.tm_mon + 1,
                                                          reviser_response->expiration_date.tm_year + 1900);
    print_menu_divider(divider_length);
    printf("= Motivazione:          %s\n",       reviser_response->motivation);
    print_menu_divider(divider_length);
    printf("= Ultimo Aggiornamento: %d/%d/%d\n", reviser_response->last_update.tm_mday,
                                                          reviser_response->last_update.tm_mon + 1,
                                                          reviser_response->last_update.tm_year + 1900);
    print_menu_divider(divider_length);
    printf("\n");
}

void print_menu_divider(int divider_length)
{
    int i;

    for(i = 0; i < divider_length; i++)
    {
        printf("=");
    }

    printf("\n");
}
