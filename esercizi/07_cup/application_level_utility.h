//
// Created by gaetano on 02/10/22.
//

#ifndef INC_07_CUP_APPLICATION_LEVEL_UTILITY_H
#define INC_07_CUP_APPLICATION_LEVEL_UTILITY_H

#include "bool_utility.h"

typedef struct __attribute__((__packed__)) request
{
    char name[255];
    char surname[255];
    char code[10];
    char exam[255];
} Request;

typedef struct __attribute__((__packed__)) response
{
    bool result;
    char exam_date[10];
} Response;

typedef struct __attribute__((__packed__)) size_list
{
    int rows;
    int columns;
} Size_list;

#endif //INC_07_CUP_APPLICATION_LEVEL_UTILITY_H
