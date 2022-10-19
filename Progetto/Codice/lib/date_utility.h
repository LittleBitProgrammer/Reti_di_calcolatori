#ifndef DATA_UTILITY_H
#define DATA_UTILITY_H

#include <time.h>                /* Importata per utilizzare la struttura "@struct tm" */
#include "bool_utility.h"    /* Importata per utilizzare il tipo di dato "@bool" */

#define MAX_YEAR 2022
#define MIN_YEAR 2020
#define MAX_DATE_LEN 11

bool input_date(struct tm*, struct tm*);
bool date_valid(int, int, int);
bool is_leap(int);
int compare_date(struct tm*, struct tm*);
struct tm add_month_to_date(struct tm, int);
char* get_timestamp(void);

#endif // DATA_UTILITY_H
