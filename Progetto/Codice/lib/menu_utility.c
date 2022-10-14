#include <stdio.h>              /*  */
#include "menu_utility.h"       /*  */
#include "data_utility.h"       /*  */
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

    fgets(verification_code, MAX_CODE_LEN, stdin);

    if(!insert_card_code(verification_code))
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
