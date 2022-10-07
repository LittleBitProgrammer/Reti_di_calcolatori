#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "date_utility.h"

bool IsDate(char * date) {
    if (strlen(date) != 10) {
        fprintf(stderr, "Attenzione formato data Errato inserire seguendo il seguente formato: DD/MM/YYYY");
        exit(EXIT_FAILURE);
    }
    char *token;
    char separatore[2] = "/";
    int giorno, mese, anno;

    token = strtok(date, separatore);
    giorno = atoi(token);

    token = strtok(NULL, separatore);
    mese = atoi(token);

    anno = atoi(token + 3);

    if (anno != 0 && mese != 0 && giorno != 0) {
        return TRUE;
    }

    return FALSE;
}