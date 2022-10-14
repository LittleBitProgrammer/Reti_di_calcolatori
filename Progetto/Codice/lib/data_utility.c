#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_utility.h"

bool input_date(struct tm* daytime, struct tm* local_time)
{
    if(scanf("%d/%d/%d", &(daytime->tm_mday), &(daytime->tm_mon), &(daytime->tm_year)) < 3)
    {
        fprintf(stderr, "Formato data invalida\n");
        return FALSE;
    }
    fflush(stdin);

    if(!date_valid(daytime->tm_mday, daytime->tm_mon, daytime->tm_year))
    {
        fprintf(stderr, "Data non valida\n");
        return FALSE;
    }

    if(local_time != NULL)
    {
        int diff_time;

        daytime->tm_mon -= 1;
        daytime->tm_year -= 1900;

        if((diff_time = compare_date(daytime, local_time)) > 31)
        {
            fprintf(stderr, "Ti sei vaccinato da oltre 31 giorni. Impossibile ottenere GreenPass\n");
            return FALSE;
        }
        else if(diff_time == -1)
        {
            fprintf(stderr, "Data di vaccinazione superiore alla data corrente\n");
            return FALSE;
        }
    }

    return TRUE;
}

bool date_valid(int day, int month, int year)
{
    if(year < MIN_YEAR || year > MAX_YEAR)
    {
        return FALSE;
    }

    if(month < 1 || month > 12)
    {
        return FALSE;
    }

    if(day < 1 || day > 31)
    {
        return FALSE;
    }

    /* Controlliamo la data in base al mese di Febbraio */
    if(month == 2)
    {
        if(is_leap(year))
        {
            return (day <= 29);
        }
        else
        {
            return (day <= 28);
        }
    }

    /* Controlliamo la data in base al mese di Aprile, Giugno, Settembre, Novembre (30 giorni) */
    if(month == 4 || month == 6 || month == 9 || month == 11)
    {
        return (day <= 30);
    }

    return TRUE;
}


/*
 * Funzione per controllare se un anno è bisestile o no, se l'anno è bisestile ritorna TRUE
 * */
bool is_leap(int year)
{
    return (((year % 4) == 0 && (year % 100) != 0) || (year % 4) == 0 && (year % 100) == 0 && (year % 400) == 0);
}

int compare_date(struct tm* vaccination_date, struct tm* local_daytime)
{
    time_t vaccination_date_time = mktime(vaccination_date);
    time_t local_time = mktime(local_daytime);

    if(vaccination_date_time > local_time)
    {
        return -1;
    }

    double seconds = difftime(local_time, vaccination_date_time);

    return (int)(seconds / (60 * 60 * 24));
}
