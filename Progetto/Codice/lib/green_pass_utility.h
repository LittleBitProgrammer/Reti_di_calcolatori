#ifndef GREEN_PASS_UTILITY_H
#define GREEN_PASS_UTILITY_H

#include "package_utility.h"

#define EXPIRATION_DURATION_MONTH 6

void populate_reviser_response(Reviser_package*);
bool check_green_pass_validity(struct tm*, struct tm*);

#endif // GREEN_PASS_UTILITY_H
