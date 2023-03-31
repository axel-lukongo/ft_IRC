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
#include <sstream>
#include "client.hpp"
#include "response.hpp"
#include "Colors.hpp"
#include "utils.hpp"
#include <signal.h>

// extern bool server_run;

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
	int _nb_client;

public:
	Server(std::string, std::string);
	void infinit_loop();
	void new_client();
	// bool is_commande(std::string);
	void client_disconnected(int);
	int	 make_command(std::string buffer, int i);
	void	SendMessage(int fd, std::string message);

	void	pass(int, std::vector<std::string>);
	void	nick(int, std::vector<std::string>);
	void	user(int, std::vector<std::string>);
	void	join(int, std::vector<std::string>);
	void	privmsg(int, std::vector<std::string>);
	int		ping_cmd(int, std::vector<std::string>);
	void	whois(int, std::vector<std::string>);
	~Server();

};


#endif //SERVER_HPP