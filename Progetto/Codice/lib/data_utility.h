#ifndef DATA_UTILITY_H
#define DATA_UTILITY_H

#include <time.h>                /* Importata per utilizzare la struttura "@struct tm" */
#include "lib/bool_utility.h"    /* Importata per utilizzare il tipo di dato "@bool" */

#define MAX_YEAR 2022
#define MIN_YEAR 2020

bool input_date(struct tm*, struct tm*);
bool date_valid(int, int, int);
bool is_leap(int);
int compare_date(struct tm*, struct tm*);

#endif // DATA_UTILITY_H
