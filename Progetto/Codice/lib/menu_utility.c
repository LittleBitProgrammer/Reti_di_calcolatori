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

    return (verify_card_code(verification_code) && accept_terms());
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

bool run_administrator_menu(Administrator_request_package* administrator_package, char* code_list, int code_size_list)
{
    int index_motivation_choice;

    print_logo();
    printf("Benvenuti sulla piattaforma Green Pass.\n\n");

    if(!code_size_list)
    {
        printf("Non ci sono elementi da aggiornare\n");
        return FALSE;
    }

    print_code_list(code_list, code_size_list);

    printf("Scegliere uno dei seguenti codici tessera sanitaria: ");

    fscanf(stdin,"%d", &(administrator_package->index_list));

    if(administrator_package->index_list < 1 || administrator_package->index_list > code_size_list)
    {
        printf("Elemento selezionato non esistente\n");
        return FALSE;
    }

    administrator_package->index_list -= 1;

    free_input_buffer();

    printf("Scegliere il codice della motivazione:\n");
    printf("1. Guarigione\n");
    printf("2. Covid\n");
    printf("> ");

    fscanf(stdin, "%d", &index_motivation_choice);

    if(index_motivation_choice < 1 || index_motivation_choice > 2)
    {
        fprintf(stderr, "Scelta selezionata errata\n");
        return FALSE;
    }

    strcpy(administrator_package->motivation, (index_motivation_choice == 1) ? "Guarigione" : "Covid");

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

void print_user_information(Reviser_package* reviser_response, int divider_length)
{
    char expiration_date[11];

    print_menu_divider(divider_length);
    printf("= Validità:             %s %s\n",    "Green Pass", reviser_response->is_green_pass_valid ? "valido" : "non valido");
    print_menu_divider(divider_length);

    snprintf(expiration_date, 11, "%d/%d/%d", reviser_response->expiration_date.tm_mday,
                                                               reviser_response->expiration_date.tm_mon + 1,
                                                               reviser_response->expiration_date.tm_year + 1900);

    printf("= Scadenza:             %s\n", reviser_response->is_green_pass_valid ?  expiration_date : "-/-/-");
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

void print_code_list(char* code_list, int size)
{
    int i;

    print_menu_divider(45);
    printf("Indice    Codici Tessera Sanitaria\n");
    print_menu_divider(45);
    for(i = 0; i < size; i++)
    {
        printf("%2d.       %s\n", i + 1, code_list + i * 21);
    }
    print_menu_divider(45);

    printf("\n");
}

bool accept_terms()
{
    int accepted_terms;
    printf("Accettare il trattamento dei dati:\n");
    printf("1. Accetto\n");
    printf("2. Non Accetto\n");
    printf(">");

    scanf("%d", &accepted_terms);

    if(accepted_terms < 1 || accepted_terms >= 2)
    {
        fprintf(stderr, "Termini e condizioni necessari per continuare ad utilizzare il software!\n");
        return FALSE;
    }
    return (accepted_terms == 1) ? TRUE : FALSE;
}