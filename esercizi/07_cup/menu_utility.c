//
// Created by gaetano on 02/10/22.
//

#include <stdio.h>
#include "menu_utility.h"

void print_menu_voices(int rows, int columns, char voices[rows][columns]){
    int i;
    for(i = 0; i < rows; i++)
    {
        printf("%d. %s\n", i + 1, voices[i]);
    }
}