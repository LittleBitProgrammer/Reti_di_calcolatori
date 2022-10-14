#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include "bool_utility.h"
#define REGION_FILE_NAME     "../file/client/tessera_sanitaria_codici_regione"
#define VACCINATED_FILE_NAME "../file/server/vaccinated"
#define MAX_FILE_LINE_SIZE 56

bool is_code_written_in_file(char*, char*);
bool subscribe_vaccinated_client(char*);

#endif //FILE_UTILITY_H
