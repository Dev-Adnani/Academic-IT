#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
#define IP "127.0.0.1"
#define PORT 5050

void send_file(FILE * fp,int sockfd){
    int n;
    char data[SIZE]={0};

    while(fgets(data,SIZE,fp) != NULL){
        printf("\n%s",data);
        if(send(sockfd,data,sizeof(data),0)==-1){
            printf("\nError while sending file.");
            return;
        }
        memset(data,0,SIZE);
    }

}

int main(){
    int sockfd,clientfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "test.txt";

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        printf("\nError while creating socket");
        return -1;
    }
    printf("\nCreated to socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    clientfd = connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(clientfd == -1){
        printf("\nError while connecting");
        return -1;
    }
    printf("\nConnected to server");

    fp=fopen(filename,"r");
    if(fp==NULL){
        printf("\nError while reading file");
        return -1;
    }

    send_file(fp,sockfd);
    printf("\nFile data sent");

    return 0;

}