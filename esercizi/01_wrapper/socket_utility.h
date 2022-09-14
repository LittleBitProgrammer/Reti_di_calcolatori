#ifndef _SOCKET_UTILITY_H_
#define _SOCKET_UTILITY_H_

#include <sys/socket.h>
#include <arpa/inet.h>

int Socket(int,int,int);
void ConnectIPV4(int,struct sockaddr_in*);
void BindIPV4(int,struct sockaddr_in*);
void Listen(int,int);
int Accept(int, struct sockaddr*, socklen_t*);

#endif