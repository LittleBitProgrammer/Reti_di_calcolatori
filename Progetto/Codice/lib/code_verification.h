#ifndef CODE_VERIFICATION_H
#define CODE_VERIFICATION_H

#include "bool_utility.h"
#include "package_utility.h"

#define MAX_CODE_LEN 21

bool verify_card_code(char*);
File_result check_region_code(char*);

#endif //CODE_VERIFICATION_H
