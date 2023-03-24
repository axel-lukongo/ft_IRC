#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "iostream"

#define PORT 8080

int main(int argc, char const **argv)
{
	int server_fd, new_socket;
	long valread;
	struct sockaddr_in address; 
	int addrlen = sizeof(address);

	std::string hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(argv[1]));

	memset(address.sin_zero, '\0', sizeof address.sin_zero);

	//we associate the socket with our address
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	int nb_connexion = 5;
	if (listen(server_fd, nb_connexion) < 0) //we lestening the information who will come on this socket
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) //here i accepte the new connexion
	{
		perror("In accept");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		
		char buffer[30000] = {0};
		
		valread = read( new_socket , buffer, 30000); //we read what we have in client socket
		
		
		std::cout << buffer << std::endl;
		// write(new_socket , "Hello from server" , hello.size());
	}
}

