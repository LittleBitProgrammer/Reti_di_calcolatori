//
// Created by gaetano on 28/09/22.
//

#include "signal_handler.h"

int served_client = 1;

void client_counter_handler(int signal_number)
{
    ++served_client;
}


