#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[1025] ;
    struct sockaddr_in servaddr;
  
    if(argc != 2) {
        fprintf(stderr,"usage: %s <IPaddress>\n",argv[0]);
        exit(1);
    }
  
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket error: ");
        exit(1);
    }
  
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(1024);
  
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        fprintf(stderr,"inet_pton error for: %s\n", argv[1]);
        exit(1);
    }

    if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Connect error: ");
        exit(1);
    }

    struct sockaddr_in my_addr;
    int len = sizeof(my_addr);
    bzero(&my_addr, len);

    getsockname(sockfd, (struct sockaddr *) &my_addr, &len);
    
    while((n = read(sockfd, recvline, 1024)) > 0) {
        recvline[n] = 0;
        
        if(fputs(recvline, stdout) == EOF) {
            fprintf(stderr,"fputs error");
            exit(1);
        }
    }

    if (n < 0) {
        printf("Read error on local port: %u\n", ntohs(my_addr.sin_port));
        exit(1);
    }  
}

