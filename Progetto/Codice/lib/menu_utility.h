#ifndef MENU_UTILITY_H
#define MENU_UTILITY_H

#include "date_utility.h"
#include "package_utility.h"

bool run_vaccinated_menu(struct tm*, struct tm*, char*);
bool run_reviser_menu(char*);
bool run_administrator_menu(Administrator_request_package*, char*, int);
void print_logo(void);
void print_user_information(Reviser_package *reviser_response, int divider_length);
void print_menu_divider(int);
void print_code_list(char*, int);
bool accept_terms(void);

#endif // MENU_UTILITY_H
