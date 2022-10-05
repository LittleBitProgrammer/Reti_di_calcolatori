#ifndef CODICE_FISCALE_H
#define CODICE_FISCALE_H

#include "bool_utility.h"

bool InsertCode(char *);
bool CheckRegion(char *);
bool StringCodeIsWritten(FILE *, char *);
bool CheckCode(char *);

#endif //CODICE_FISCALE_H
