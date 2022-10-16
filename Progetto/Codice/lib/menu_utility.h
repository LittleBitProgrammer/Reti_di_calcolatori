#ifndef MENU_UTILITY_H
#define MENU_UTILITY_H

#include "date_utility.h"
#include "package_utility.h"

bool run_vaccinated_menu(struct tm*, struct tm*, char*);
bool run_reviser_menu(char*);
bool run_administrator_menu(char*, char*);
void print_logo(void);
void print_reviser_result(Reviser_package*, int);
void print_menu_divider(int);

#endif // MENU_UTILITY_H
