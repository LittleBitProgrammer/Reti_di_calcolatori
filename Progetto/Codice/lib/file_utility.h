#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include "bool_utility.h"
#include "package_utility.h"

#define REGION_FILE_NAME     "../file/client/tessera_sanitaria_codici_regione"
#define VACCINATED_FILE_NAME "../file/server/vaccinated"
#define MAX_FILE_LINE_SIZE 56

File_result is_code_written_in_file(char*, char*);
File_result subscribe_vaccinated_client(char*);
int count_file_lines(char*, char**);
bool change_information_in_file(Administrator_request_package*, Administrator_response_package*, char*);

#endif //FILE_UTILITY_H
