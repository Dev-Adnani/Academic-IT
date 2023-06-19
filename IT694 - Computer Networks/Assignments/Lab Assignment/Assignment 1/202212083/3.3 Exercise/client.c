// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char* middle = "What are you doing?";
	char* bye = "Cool!";
	char* end = "Byee!";


	char buffer[1024] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd
		= connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");

	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	send(sock, middle, strlen(hello), 0);
	printf("What are you doing? sent\n");

	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	send(sock, bye, strlen(hello), 0);
	printf("Cool! \n");

	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	send(sock, end, strlen(hello), 0);
	printf("Byee! sent\n");

	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	// closing the connected socket
	close(client_fd);
	return 0;
}

