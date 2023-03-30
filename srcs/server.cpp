#include "../include/server.hpp"

//! ############### CONSTRUCTOR ####################

<<<<<<< HEAD:server.cpp
Server::Server(std::string argv, std::string pwd): _addrlen(sizeof(_address)), _pwd(pwd)
=======
Server::Server(std::string argv): _addrlen(sizeof(_address)), _nb(0)
>>>>>>> main:srcs/server.cpp
{
	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	/**
	 * SO_REUSEADDR this option allow us to reuse the address of the socket
	   if we close our socket

	 * SOL_SOCKET: it a constante who allow us to configure the option of the socket

	 * setsockopt: this fonction it for appli the SO_REUSEADDR on my server_fd
	 */
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	//this variable it just the variable in my struct sockaddr_in
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(atoi(argv.c_str()));

	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

	//we associate the socket with our address
	if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	int nb_connexion = 10;
	if (listen(_server_fd, nb_connexion) < 0) //we lestening the information who will come on this socket
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	_fds[0].fd = _server_fd; //the fd in index 0 is the socket of my server
<<<<<<< HEAD:server.cpp
	_fds[0].events = POLLIN;

=======
	_fds[0].events = POLLIN; 
	_nb++;
>>>>>>> main:srcs/server.cpp
	infinit_loop();
}

void	Server::SendMessage(int fd, std::string message)
{
<<<<<<< HEAD:server.cpp
	std::cout << BLUE "[SERVER] >> " << RESET << message << std::endl;
	send(fd, message.c_str(), message.size(), 0);
}

//! ############### END CONSTRUCTOR ####################

//! ############### LOOP ####################

void Server::infinit_loop()
{
	while(1)
	{
		//this function it supposed to watch if we have any activity on our Server_fd
		_watch_activity = poll(_fds, _clients.size() + 1, -1);
		if (_watch_activity < 0)
		{
			perror("poll failed");
			exit(EXIT_FAILURE);
		}
		//POLLIN is a constant that allow us to watch if something trop to 
		// communicate or interact with us
		//here i check if someone try to interacte with my server
		if (_fds[0].revents == POLLIN) {
			std::cout << "new client\n" << std::endl;
			new_client();
		}

		/*in this loop i watch if one of my client send something
		so i going check them one by one*/
		for (size_t i = 1; i <= _clients.size(); i++) {
			if (_fds[i].revents == POLLIN) { //here i check if on of my client try to do something
				char buffer[30000] = {0};
				//if we POLLIN but read return 0 it mean this client is disconnect
				if ((_valread = read(_fds[i].fd , buffer, 30000)) == 0) {
					client_disconnected(i);
					continue; /*(continute) it a keyword who allow us to jump directely
					to the next iteration without to executate the continuation of the code*/ 
				}

				/*so we going to come down here only if the interact detecte
				by POLLIN wasn't a disconnection of the client */

				// Get client namename
				if (_clients[i-1].nickname == "unknown") {
					_clients[i-1].nickname = std::to_string(i)[0];
					std::cout << "Client: " << _clients[i-1].nickname << " is connect\n";
				}
				//on pourra parser le message pour savoir si c'est une commande ici
				//si buffer[0] == '/'
				make_command(buffer, i);
				std::cout << _clients[i-1].nickname << ": " << buffer << std::endl;
			}
=======
	//this function it supposed to watch if we have any activity on our Server_fd
	_watch_activity = poll(_fds, _clients.size() + 1, -1);
	if (_watch_activity < 0) {
		perror("poll failed");
		exit(EXIT_FAILURE);
	}
	//POLLIN is a constant that allow us to watch if something trop to 
	// communicate or interact with us
	//here i check if someone try to interacte with my server
	if (_fds[0].revents == POLLIN) {
		new_client();
		// std::cout << "------ " << _clients[0].name << std::endl;
		_nb++;
		continue;
	}

	/*in this loop i watch if one of my client send something
	so i going check them one by one*/
	for (size_t i = 1; i <= _clients.size(); i++) {
		if (_fds[i].revents == POLLIN) { //here i check if on of my client try to do something
			char buffer[512] = {0};
			//if we POLLIN but read return 0 it mean this client is disconnect
			if ((_valread = recv(_fds[i].fd , buffer, 512, 0)) == 0) {
				client_disconnected(i);
				continue; /*(continute) it a keyword who allow us to jump directely
				to the next iteration without to executate the continuation of the code*/ 
			}

			/*so we going to come down here only if the interact detecte
			by POLLIN wasn't a disconnection of the client */

			// Get client namename
			if (_clients[i-1].name == "unknown") {
					std::stringstream int_to_str;
					int_to_str << i;
					_clients[i - 1].name = int_to_str.str();
				std::cout << "Client: " << _clients[i-1].name << " is connect\n";
			}
			//on pourra parser le message pour savoir si c'est une commande ici
			//si buffer[0] == '/'
			if(is_commande(buffer) == false)
				std::cout << _clients[i-1].name << ": " << buffer << std::endl;
>>>>>>> main:srcs/server.cpp
		}
	}
}

void Server::new_client(){
	if ((_new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen))<0)
	{
		perror("In accept");
		exit(EXIT_FAILURE);
	}

<<<<<<< HEAD:server.cpp
	// Craft the reply message
	// std::string replyMessage = ":localhost 001 ngobert :Welcome to the IRC server ircserv!\r\n";

	// Send the message to the client
	// send(_new_socket, replyMessage.c_str(), replyMessage.length(), 0);

	std::string name = "unknown";
	Client client = { _new_socket, name };
	client.nickname = "unknown";
	client.name = "";
=======
	std::stringstream int_to_str;
	int_to_str << _nb;
	std::string the_name = int_to_str.str();
	Client client = { _new_socket, the_name };
>>>>>>> main:srcs/server.cpp
	_clients.push_back(client);

	_fds[_clients.size()].fd = _new_socket;
	_fds[_clients.size()].events = POLLIN;
	std::cout << "-----------new client connect--------------\n";
}



void Server::client_disconnected(int i){
	close(_fds[i].fd);
<<<<<<< HEAD:server.cpp
	_clients.erase(_clients.begin() + (i - 1));
	_fds[i].fd = -1;
	std::cout << "Client " << _clients[i - 1].nickname << "is disconnect";
=======
	_clients.erase(_clients.begin() + i );
	_fds[i].fd = 0;
	std::cout << "Client " << _clients[i-1].name << " is disconnect \n\n";

>>>>>>> main:srcs/server.cpp
}

bool Server::is_commande(std::string buffer){
	if(buffer[0] == '/'){
		std::cout << "is a commande\n";
		return true;
	}
	else
		return false;
}

Server::~Server()
{
}
