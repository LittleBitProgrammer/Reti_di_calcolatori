/**
 * @file file_utility.h
 * @author  Roberto Vecchio, Francesco Mabilia & Gaetano Ippolito
 * @brief   La seguente libreria ha lo scopo di incapsulare funzioni legate alla gestione dei file
 * 
 * @type    Libreria
 * @version 1.0
 */

/* Direttive precompilatore volte a definire la libreria @file_utility.h */
#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

/* 
 * ==========================
 * =         Import         =
 * ==========================
 */
#include "bool_utility.h"
#include "package_utility.h"

/* 
 * ==========================
 * =       Constants        =
 * ==========================
 */
#define REGION_FILE_NAME     "../file/client/tessera_sanitaria_codici_regione"
#define VACCINATED_FILE_NAME "../file/server/vaccinated"
#define MAX_FILE_LINE_SIZE 56

/* 
 * ================================
 * =     Function Definition      =
 * ================================
 */
File_result is_code_written_in_file(char*, char*, int);
File_result subscribe_vaccinated_client(char*);
int read_lines(char*, char**);
bool change_information_in_file(Administrator_request_package*, Administrator_response_package*, char*);

/* Direttiva precompilatore utilizzata per indicare la delimitazione di fine libreria @file_utility.h */
#endif //FILE_UTILITY_H
