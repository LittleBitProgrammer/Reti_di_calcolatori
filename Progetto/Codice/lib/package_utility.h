#ifndef PACKAGE_UTILITY_H
#define PACKAGE_UTILITY_H

#include <time.h>

typedef struct __attribute__((__packed__))
{
    char card_code[21];
    struct tm vaccination_date;
} Vaccinated_package;

#endif //PACKAGE_UTILITY_H
