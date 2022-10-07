//
// Created by francesco on 04/10/22.
//
#include <stdio.h>
#include <stdlib.h>
#include "codice_fiscale.h"

#define  MAX_SIZE 100

int main()
{
    char *codice = (char *) malloc(MAX_SIZE *sizeof(char));
    bool verify;

    printf("Inserire il codice: ");
    scanf("%s", codice);

    verify = VerifyCod(codice);

    printf("verify = %s\n", verify == 0 ? "FALSE" : "TRUE");

    free(codice);

    exit(EXIT_SUCCESS);
}

/*
 * xxxxx-xxxxx-xxxxxxxxxx
 * */