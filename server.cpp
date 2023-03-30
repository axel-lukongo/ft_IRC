#include"server.hpp"

//! ############### CONSTRUCTOR ####################

Server::Server(std::string argv, std::string pwd): _addrlen(sizeof(_address)), _pwd(pwd)
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
	_address.sin_port = htons(stoi(argv));

	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

	//we associate the socket with our address
	if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address))<0)
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
	_fds[0].events = POLLIN;

	infinit_loop();
}

void	Server::SendMessage(int fd, std::string message)
{
	std::cout << BLUE "[SERVER] >> " << RESET << message << std::endl;
	send(fd, message.c_str(), message.size(), 0);
}

//! ############### END CONSTRUCTOR ####################

//! ############### COMMANDS ####################

	//? PASS ####################################
void	Server::pass(int i, std::vector<std::string> command_split)
{
	if (command_split[1] == this->_pwd)
	{
		_clients[i - 1].is_registered = true;
		SendMessage(_clients[i - 1].fd, RPL_WELCOME(_clients[i - 1].nickname, _clients[i - 1].nickname));
	}
	else
	{
		std::cout << BLUE "[PASS TEST]"<< RESET << std::endl;
		SendMessage(_clients[i - 1].fd, ERR_PASSWDMISMATCH(_clients[i - 1].nickname));
	}
}

	//? NICK ####################################
void	Server::nick(int i, std::vector<std::string> command_split)
{
	_clients[i - 1].old_nickname = _clients[i - 1].nickname;
	_clients[i - 1].nickname = command_split[1];
	SendMessage(_clients[i - 1].fd, RPL_NICK(_clients[i - 1].old_nickname, _clients[i - 1].nickname, _clients[i - 1].nickname));
}

	//? USER ####################################
void	Server::user(int i, std::vector<std::string> command_split)
{
	_clients[i - 1].username = command_split[1];
	_clients[i - 1].hostname = command_split[2];
	_clients[i - 1].servername = command_split[3];
	_clients[i - 1].realname = command_split[4];
	SendMessage(_clients[i - 1].fd, RPL_YOURHOST(_clients[i - 1].nickname, _clients[i - 1].servername));
	SendMessage(_clients[i - 1].fd, RPL_CREATED(_clients[i - 1].nickname, _clients[i - 1].servername));
	SendMessage(_clients[i - 1].fd, RPL_MYINFO(_clients[i - 1].nickname, _clients[i - 1].servername, "0.0.1", "0.0.1"));
}

	//? JOIN ####################################
void	Server::join(int i, std::vector<std::string> command_split)
{
	_clients[i - 1].channel = command_split[1];
	_clients[i - 1].channels_joined.push_back(_clients[i - 1].channel);
	SendMessage(_clients[i - 1].fd, RPL_TOPIC(_clients[i - 1].nickname, _clients[i - 1].channel, "No topic is set"));
	SendMessage(_clients[i - 1].fd, RPL_NAMREPLY(_clients[i - 1].nickname, _clients[i - 1].channel, _clients[i - 1].nickname));
	SendMessage(_clients[i - 1].fd, RPL_ENDOFNAMES(_clients[i - 1].nickname, _clients[i - 1].channel, "End of /NAMES list"));
}

	//? PRIVMSG ####################################
void	Server::privmsg(int i, std::vector<std::string> command_split)
{
	std::string channel = command_split[1];
	std::string nickname = _clients[i - 1].nickname;

	//take the rest of the arguments and put it in the message
	std::string tmp;
	for (size_t j = 2; j < command_split.size(); j++)
	{
		tmp += command_split[j];
		tmp += " ";
	}

	if (command_split[1][0] == '#')
	{
		for (size_t j = 0; j < _clients.size(); j++)
		{
			if (_clients[j].channel == channel && _clients[j].nickname != nickname)
				SendMessage(_clients[j].fd, RPL_PRIVMSG(nickname, channel, tmp));
		}
	}
	else // send a message to another client
	{
		for (size_t j = 0; j < _clients.size(); j++)
		{
			if (_clients[j].nickname == channel && _clients[j].nickname != nickname)
			{
				SendMessage(_clients[j].fd, RPL_PRIVMSG(nickname, channel, tmp));
			}
		}
	}
}



//! ############### END COMMANDS ####################

//! ############### MAKE_COMMAND ####################

int	Server::make_command(std::string buffer, int i)
{
	//Go through the line and find every command and its arguments and put the command and its arguments in the same vector and the next command and it's arguments in the next vector
	std::vector<std::string> command;
	std::string tmp;
	for (size_t j = 0; j < buffer.size(); j++)
	{
		if (buffer[j] == '\n')
		{
			command.push_back(tmp);
			tmp = "";
		}
		else
			tmp += buffer[j];
	}
	tmp = deleteFlags(tmp);
	command.push_back(tmp);
	//Print the commands and their arguments
	for (size_t j = 0; j < command.size(); j++)
		std::cout << command[j] << std::endl;
	//Execute the commands
	for (size_t j = 0; j < command.size(); j++)
	{
		//Split the command and its arguments
		std::vector<std::string> command_split;
		std::string tmp;
		for (size_t k = 0; k < command[j].size(); k++)
		{
			if (command[j][k] == ' ' || command[j][k] == '\r')
			{
				command_split.push_back(tmp);
				tmp = "";
			}
			else
				tmp += command[j][k];
		}
		command_split.push_back(tmp);
		//Execute the command
		if (command_split[0] == "NICK")
			nick(i, command_split);
		else if (command_split[0] == "PASS")
			pass(i, command_split);
		else if (command_split[0] == "USER")
			user(i, command_split);
		else if (command_split[0] == "JOIN")
			join(i, command_split);
		else if (command_split[0] == "PRIVMSG")
			privmsg(i, command_split);
	}
	return (0);
}

//! ############### END MAKE_COMMAND ####################

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
		}
	}
}



void Server::new_client(){
	if ((_new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen))<0)
	{
		perror("In accept");
		exit(EXIT_FAILURE);
	}

	// Craft the reply message
	// std::string replyMessage = ":localhost 001 ngobert :Welcome to the IRC server ircserv!\r\n";

	// Send the message to the client
	// send(_new_socket, replyMessage.c_str(), replyMessage.length(), 0);

	std::string name = "unknown";
	Client client = { _new_socket, name };
	client.nickname = "unknown";
	client.name = "";
	_clients.push_back(client);

	_fds[_clients.size()].fd = _new_socket;
	_fds[_clients.size()].events = POLLIN;
	std::cout << "-----------new client connect--------------\n";
}



void Server::client_disconnected(int i){
	close(_fds[i].fd);
	_clients.erase(_clients.begin() + (i - 1));
	_fds[i].fd = -1;
	std::cout << "Client " << _clients[i - 1].nickname << "is disconnect";
}



Server::~Server()
{
}
