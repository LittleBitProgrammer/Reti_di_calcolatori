#include <ctype.h>
#include "digits_utility.h"

/**
 * @brief
 *
 * @param code
 *
 * @return 
 * */
bool is_digits_only(const char* code)
{
    while (*code)
    {
        //TODO: tradurre
        /**
         * Function isdigit() takes a single argument in the form of an integer and returns the value of type int.

         *  Even though, isdigit() takes integer as an argument, character is
         *  passed to the function. Internally, the character is converted to its
         *  ASCII value for the check.
         *
         *  @return Non-zero integer ( x > 0 ) if argument is a numeric character else Zero (0) if argument is not a numeric character
         *
         * */
        if (isdigit(*code++) == 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

