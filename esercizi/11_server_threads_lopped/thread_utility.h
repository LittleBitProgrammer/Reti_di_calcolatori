//
// Created by francesco on 10/10/22.
//

#ifndef THREADS_THREAD_UTILITY_H
#define THREADS_THREAD_UTILITY_H

#include <netinet/in.h>

#define MAX_THREAD 64

typedef struct
{
    int                 arg1;
    struct sockaddr_in  arg2;

} arg_struct;

void* server_handler(void*);


#endif //INC_10_SERVER_THREADS_THREAD_UTILITY_H
