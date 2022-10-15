#ifndef GREEN_PASS_UTILITY_H
#define GREEN_PASS_UTILITY_H

#include "package_utility.h"

#define EXPIRATION_DURATION_MONTH 6

File_flags* generate_reviser_response(Reviser_package *reviser_package, char *card_code);
bool check_green_pass_validity(struct tm*, struct tm*, char*);

#endif // GREEN_PASS_UTILITY_H
