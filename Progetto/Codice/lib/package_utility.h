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
    bool open_file_flag;
    bool write_file_flag;
} File_flags;

typedef struct
{
    bool result_flag;
    File_flags file_flags;
} File_result;

typedef struct
{
    bool is_green_pass_valid;
    struct tm expiration_date;
    struct tm last_update;
    char* motivation;
    File_flags file_flags;
} Reviser_package;

#endif //PACKAGE_UTILITY_H
