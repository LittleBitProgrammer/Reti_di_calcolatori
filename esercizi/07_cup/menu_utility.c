//
// Created by gaetano on 02/10/22.
//

#include <stdio.h>
#include "menu_utility.h"

void print_menu()
{
    printf("=======================================================\n");
    printf("Scegliere una delle seguenti opzioni:\n\n");
    printf("\t[1] Prenotazione esame.\n");
    printf("\t[2] Ricarica la lista degli esami disponibili.\n");
    printf("\t[3] Esci.\n");
    printf("=======================================================\n");

    printf("\nInserire scelta: ");
}

void print_menu_voices(int rows, int columns, char voices[rows][columns]){
    int i;
    for(i = 0; i < rows; i++)
    {
        printf("%d. %s\n", i + 1, voices[i]);
    }
}

void booking_request(Request* request)
{
    printf("\nCompilare la prenotazione\n");
    printf("Nome: ");
    scanf("%s", request->name);

    printf("Cognome: ");
    scanf("%s", request->surname);

    printf("Codice: ");
    scanf("%s", request->code);

    printf("Esame: ");
    scanf("%s", request->exam);
}