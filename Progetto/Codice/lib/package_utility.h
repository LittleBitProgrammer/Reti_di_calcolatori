#ifndef PACKAGE_UTILITY_H
#define PACKAGE_UTILITY_H

#include <time.h>
#include "bool_utility.h"

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

typedef struct
{
    bool result_flag;
    bool open_file_flag;
    bool write_file_flag;
    bool read_file_flag;
} Error_handling;

#endif //PACKAGE_UTILITY_H
