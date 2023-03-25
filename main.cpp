#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include "iostream"
#include <vector>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.hpp"
// #define PORT 8080

// struct Client {
//     int fd;
//     std::string name;
// };

int main(int argc, char const **argv)
{
	Server Server(argv[1]);
// 	int server_fd, new_socket;
// 	long valread;
// 	struct sockaddr_in address;
// 	int addrlen = sizeof(address);
// 	struct pollfd fds[10 + 1]; //this struct it for watche and add many socket_fd in it
// 	std::vector<Client> clients;

// 	// Creating socket file descriptor
// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	{
// 		perror("In socket");
// 		exit(EXIT_FAILURE);
// 	}

// 	int opt = 1;
// 	/**
// 	 * SO_REUSEADDR this option allow us to reuse the address of the socket
// 	   if we close our socket

// 	 * SOL_SOCKET: it a constante who allow us to configure the option of the socket

// 	 * setsockopt: this fonction it for appli the SO_REUSEADDR on my server_fd
// 	 */
// 	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
// 		perror("setsockopt failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(atoi(argv[1]));

// 	memset(address.sin_zero, '\0', sizeof address.sin_zero);

// 	//we associate the socket with our address
// 	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
// 	{
// 		perror("In bind");
// 		exit(EXIT_FAILURE);
// 	}

// 	int nb_connexion = 10;
// 	if (listen(server_fd, nb_connexion) < 0) //we lestening the information who will come on this socket
// 	{
// 		perror("In listen");
// 		exit(EXIT_FAILURE);
// 	}

// 	fds[0].fd = server_fd;
// 	fds[0].events = POLLIN;




// int watch_activity;
// while(1)
// {
//     //this function it supposed to watch if we have any activity on our server_fd
//     watch_activity = poll(fds, clients.size() + 1, -1);
//     if (watch_activity < 0) {
//         perror("poll failed");
//         exit(EXIT_FAILURE);
//     }
//     if (fds[0].revents & POLLIN) {
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
//         {
//             perror("In accept");
//             exit(EXIT_FAILURE);
//         }
// 		std::cout << "-----------new client connect--------------\n";

//         std::string name = "unknown";
//         Client client = { new_socket, name };
//         clients.push_back(client);

//         fds[clients.size()].fd = new_socket;
//         fds[clients.size()].events = POLLIN;

//         // std::cout << "New client connected with fd: " << new_socket;
//     }
// 			std::cout << "----------ici----------\n";

//     for (int i = 1; i <= clients.size(); i++) {
//         if (fds[i].revents & POLLIN) {
//             char buffer[30000] = {0};
//             if ((valread = read(fds[i].fd , buffer, 30000)) == 0) {
//                 // Client disconnected
//                 close(fds[i].fd);
//                 clients.erase(clients.begin() + (i - 1));
//                 fds[i].fd = -1;
//                 std::cout << "Client with fd %d disconnected\n" << fds[i].fd;
//                 continue;
//             }

//             // Get client namename
//             if (clients[i-1].name == "unknown") {
//                 clients[i-1].name = std::to_string(i)[0];
//                 std::cout << "Client: " << clients[i-1].name << " is connect\n";
//             } else {
//                 // Broadcast message to all connected clients
//                 std::cout << clients[i-1].name << ": " << buffer << std::endl;
//                 for (int j = 0; j < clients.size(); j++) {
//                     if (clients[j].fd != fds[i].fd && clients[j].name != "unknown") {
//                         if (send(clients[j].fd, buffer, strlen(buffer), 0) == -1) {
//                             perror("send failed");
//                             close(clients[j].fd);
//                             clients.erase(clients.begin() + j);
//                             j--;
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

}




// struct Client {
//     int fd;
//     std::string name;
// };


// int main(int ac, char **av) {
//     int server_fd, activity, i, valread;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     char buffer[1024] = {0};
//     std::vector<Client> clients;
    // struct pollfd fds[MAX_CLIENTS + 1];

//     // Create a TCP socket
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // Set socket options
//     int opt = 1;
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
//         perror("setsockopt failed");
//         exit(EXIT_FAILURE);
//     }

//     // Bind socket to port
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(atoi(av[1]));

//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     // Listen for incoming connections
//     if (listen(server_fd, 3) < 0) {
//         perror("listen failed");
//         exit(EXIT_FAILURE);
//     }

//     // Add server socket to pollfd array
//     fds[0].fd = server_fd;
//     fds[0].events = POLLIN;

//     std::cout << "Server started. Listening on port " << atoi(av[1]) << "...\n";

//     while (1) {
//         // Wait for activity on any socket
//         activity = poll(fds, clients.size() + 1, -1);
//         if (activity < 0) {
//             perror("poll failed");
//             exit(EXIT_FAILURE);
//         }

//         // Check for incoming connection requests
//         if (fds[0].revents & POLLIN) {
//             int new_socket;
//             if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
//                 perror("accept failed");
//                 exit(EXIT_FAILURE);
//             }
//             std::cout << "New client connected. IP address: " << inet_ntoa(address.sin_addr) << ", Port: " << ntohs(address.sin_port) << "\n";

//             // Add new client to clients vector and pollfd array
//             Client new_client;
//             new_client.fd = new_socket;
//             clients.push_back(new_client);

//             int index = clients.size();

//             fds[index].fd = new_socket;
//             fds[index].events = POLLIN;

//             // Send welcome message to client
//             std::string welcome_msg = "Welcome to the server!\n";
//             send(new_socket, welcome_msg.c_str(), welcome_msg.length(), 0);
//         }
// 		// Check for messages from clients
// 		for (i = 0; i < clients.size(); i++) {
// 			if (fds[i+1].revents & POLLIN) {
// 				// Read message from client
// 				valread = read(fds[i+1].fd, buffer, 1024);

// 				if (valread == 0) {
// 					// Client disconnected
// 					close(fds[i+1].fd);
// 					std::cout << "Client disconnected. namename: " << clients[i].name << "\n";
// 					clients.erase(clients.begin() + i);
// 					fds[i+1].fd = -1;
// 				} else {
// 					// Broadcast message to all clients
// 					std::string message = clients[i].name + ": " + buffer;
// 					for (int j = 0; j < clients.size(); j++) {
// 						if (fds[j+1].fd != -1 && j != i) {
// 							send(fds[j+1].fd, message.c_str(), message.length(), 0);
// 						}
// 					}
// 				}
// 			}
// 		char buff[30000] = {0};

// 		valread = read( fds[i].fd , buff, 30000); //we read what we have in client socket

// 		// std::cout << "j'attend au 2\n";

// 		std::cout << buff << std::endl;
// 		}

// 	}
// }