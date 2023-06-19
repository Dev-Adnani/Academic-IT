#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>

#define PORT 5050
#define SIZE 1024

void write_file(int sockfd){
    int n;
    FILE *fp;
    char * filename = "get.txt";
    char buffer[SIZE];

    fp = fopen(filename,"w");
    while(1){
        n = recv(sockfd,buffer,SIZE,0);
        if(n<=0){break;return;}
        printf("\n%s",buffer);
        fprintf(fp,"%s",buffer);
        memset(buffer,0,1024);

    }
    return;
}

int main(){
	int server_fd,new_socket,valRead;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024];
	char *hello = "Hello from server";
	
	//creating server socket using socket
	if((server_fd = socket(AF_INET,SOCK_STREAM,0))==0){
		//printing error message and return if fails to create socket
		printf("\nSocket creation error");
		return -1;
	}
	
	printf("\nSocket Created");
	if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt))){
		printf("\nSetsockopt");
		return -1;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	//binding socket to port
	if(bind(server_fd,(struct sockaddr *)&address , sizeof(address))<0){
		//printing error message and return if socket fails to bind to the port
		printf("\nBinding Error");
		return -1;
	}
	printf("\nSocket Binded");
	
	//listening on port using server socket
	if(listen(server_fd,3)<0){
		//printing error message and return if there is an error 
		//while listening for connections
		printf("\nListening Error");
		return -1;
	}
	printf("\nSocket Listening");
	
	//accepting new conection request from clients
	if((new_socket = accept(server_fd,(struct sockaddr *)&address, (socklen_t *)&addrlen))<0){
		//printing error message and return if there is an error
		//while accepting clients connection
		printf("\nAccepting Error");
		return -1;
}
	printf("\nSocket Accepted request\n");
	write_file(new_socket);
    printf("Data saved");
    
	return 0;
}