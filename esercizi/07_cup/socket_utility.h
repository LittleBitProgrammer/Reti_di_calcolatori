#ifndef SOCKET_UTILITY_H
#define SOCKET_UTILITY_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "bool_utility.h"

int Socket(int,int,int);
void ConnectIPV4(int,struct sockaddr_in*);
void BindIPV4(int,struct sockaddr_in*);
void Listen(int,int);
int AcceptIPV4(int, struct sockaddr_in*, socklen_t*);
void PrintClientIPV4(struct sockaddr_in*, char *, socklen_t);
size_t FullWrite(int, const void *, size_t);
size_t FullRead(int, void *, size_t);
bool check_connection(int);

#endif