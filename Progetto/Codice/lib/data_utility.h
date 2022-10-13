#ifndef DATA_UTILITY_H
#define DATA_UTILITY_H

#include <time.h>               /* Importata per utilizzare la struttura "@struct tm" */
#include "lib/bool_utility.h"

bool input_date(struct tm*);
bool is_valid_date(struct tm*);

#endif // DATA_UTILITY_H
