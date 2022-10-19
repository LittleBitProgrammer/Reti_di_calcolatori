#ifndef THREAD_UTILITY_H
#define THREAD_UTILITY_H

#include <pthread.h>
#include "bool_utility.h"

#define MAX_THREADS 64

typedef struct
{
    int file_descriptor;
    struct sockaddr_in* endpoint;
} Args;

void* vaccination_center_handler(void*);
void* central_server_handler(void*);
void* assistant_server_handler(void*);
bool is_main_thread(void);

#endif // THREAD_UTILITY_H
