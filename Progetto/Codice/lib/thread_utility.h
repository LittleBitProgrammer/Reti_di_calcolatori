#ifndef THREAD_UTILITY_H
#define THREAD_UTILITY_H

#include <pthread.h>
#define MAX_THREADS 64

typedef struct
{
    int arg1;
    struct sockaddr_in arg2;
} Arguments;

void* vaccination_center_handler(void*);
void* central_server_handler(void*);
void* assistant_server_handler(void*);

#endif // THREAD_UTILITY_H
