#ifndef PACKAGE_UTILITY_H
#define PACKAGE_UTILITY_H

#include <time.h>

typedef struct
{
    char card_code[21];
    struct tm vaccination_date;
} Vaccinated_package;

typedef struct
{
    Vaccinated_package vaccinated_package;
    struct tm expiration_date;
} Subscribe_package;

#endif //PACKAGE_UTILITY_H
