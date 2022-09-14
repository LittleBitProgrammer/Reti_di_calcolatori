#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFSIZE 4096
#define BACKLOG_SIZE 1

int main(int argc, char **argv) {
    int listenfd, connfd;
    
    struct sockaddr_in servaddr, clientaddr, clientremote;
    socklen_t size = sizeof(clientaddr);

    char buff[BUFFSIZE];
    time_t ticks;
  
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(1024);

    if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Bind error: ");
        exit(1);
    }

    if (listen(listenfd, BACKLOG_SIZE) < 0) {
        perror("Listen error: ");
        exit(1);
    }

    for( ; ; ) {
        if((connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &size)) < 0 ) {
            perror("Accept error: ");
            exit(1);
        }

        sleep(10);

        socklen_t len = sizeof(clientremote);
        bzero(&clientremote, len);
        
        getpeername(connfd, (struct sockaddr*) &clientremote, &len);
        fprintf(stderr, "Serving client with port: %u\n", ntohs(clientremote.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        
        if(write(connfd, buff, strlen(buff)) != strlen(buff)) {
            perror("Write error: ");
            exit(1);
        }

        close(connfd);
    }
}

