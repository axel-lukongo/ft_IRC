#ifndef SERVER_HPP
#define SERVER_HPP
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
#include "client.hpp"
#include "response.hpp"
#include "Colors.hpp"

class Server
{
private:
	std::string _pwd;
	int _server_fd, _new_socket;
	long _valread;
	struct sockaddr_in _address;
	struct pollfd _fds[10 + 1]; //this struct it for watche and add many socket_fd in it
	int _addrlen;
	std::vector<Client> _clients;
	int _watch_activity;

public:
	Server(std::string, std::string);
	void infinit_loop();
	void new_client();
	void client_disconnected(int);
	int	 make_command(std::string buffer, int i);
	void	SendMessage(int fd, std::string message);
	~Server();
};

#endif // !SERVER_HPP