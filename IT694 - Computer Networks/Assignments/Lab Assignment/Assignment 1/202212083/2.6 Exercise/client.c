#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
int main(int argc , char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *exit_msg = "exit" , *msg;
	char buffer[1024]={0};
	if((sock = socket(AF_INET, SOCK_STREAM , 0))<0)
	{
		printf("\n Socket creation error\n");
		return -1;
	}
	serv_addr.sin_family =AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\n Invalid address / address not supported \n");
		return -1;
	}
	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	{
		printf("\n connection Failed \n");
		return -1;
	}
	while(1){
		scanf("%s",msg);
		if(!strcmp(msg, exit_msg)){
			close(sock);
			return 0;
		}
		send(sock, msg , strlen(msg) , 0);
		valread = read(sock, buffer, 1024);
		printf("%s\n",buffer);
	}
	return 0;
}
