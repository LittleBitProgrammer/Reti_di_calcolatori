#include <stdio.h>
#include "buffer_utility.h"

void free_input_buffer()
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF);
}