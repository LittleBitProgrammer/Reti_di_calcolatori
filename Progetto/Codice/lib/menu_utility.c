#include <stdio.h>            /*  */
#include "menu_utility.h"     /*  */
#include "data_utility.h"     /*  */

/**
 * @brief
 *
 * @param vaccination_date
 * @param  local_daytime
 *
 * @return
 * */
bool run_vaccinated_menu(struct tm* vaccination_date, struct tm* local_daytime)
{
    print_logo();
    printf("Benvenuti nel centro vaccinale.\n\n");
    printf("Inserire data di vaccinazione nel formato DD/MM/YYYY: ");

    if(!input_date(vaccination_date, local_daytime))
    {
        return FALSE;
    }

    /*TODO: Aggiungere possibilità di mettere il codice*/
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
