#include <stdio.h>
#include "data_utility.h"

bool input_date(struct tm* daytime)
{
    bool is_valid_date = TRUE;

    if(scanf("%d/%d/%d", &(daytime->tm_mday), &(daytime->tm_mon), &(daytime->tm_year)) < 3)
    {
        return !is_valid_date;
    }

    return is_valid_date;
}

bool is_valid_date(struct tm* daytime)
{
    
}
