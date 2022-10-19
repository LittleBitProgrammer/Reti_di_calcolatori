#include <stdio.h>              /*  */
#include <string.h>             /*  */
#include "date_utility.h"       /*  */

/**
 * @brief
 *
 * @param daytime
 * @param local_time
 *
 * @return
 * */
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

/**
 * @brief
 *
 * @param day
 * @param month
 * @param year
 *
 * @return
 * */
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

/**
 * @brief Funzione per controllare se un anno è bisestile o no, se l'anno è bisestile ritorna TRUE
 *
 * @param year
 *
 * @return
 * */
bool is_leap(int year)
{
    return (((year % 4) == 0 && (year % 100) != 0) || (year % 4) == 0 && (year % 100) == 0 && (year % 400) == 0);
}

/**
 * @brief
 *
 * @param date_1
 * @param date_2
 *
 * @return
 * */
int compare_date(struct tm* date_1, struct tm* date_2)
{
    time_t vaccination_date_time = mktime(date_1);
    time_t local_time = mktime(date_2);

    if(vaccination_date_time > local_time)
    {
        return -1;
    }

    double seconds = difftime(local_time, vaccination_date_time);

    return (int)(seconds / (60 * 60 * 24));
}

/**
 * @brief
 *
 * @param vaccination_date
 *
 * @return
 * */
struct tm add_month_to_date(struct tm vaccination_date, int months)
{
    struct tm expiration_date = vaccination_date;

    expiration_date.tm_mon += months;
    mktime(&expiration_date);

    return expiration_date;
}

char* get_timestamp(void)
{
    time_t timestamp = time(NULL);
    char *daytime = ctime(&timestamp);

    daytime[strlen(daytime)-1] = 0;

    return daytime;
}
