/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:09:59 by ngobert           #+#    #+#             */
/*   Updated: 2023/03/31 22:17:03 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../include/the_include.hpp"

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
	// std::cout << "\n\n\n --------------le chanel est: " << command_split[1] << "--------------\n\n\n";
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


	//? PING ####################################
int	Server::ping_cmd(int i, std::vector<std::string> command_split)
{
	// Client		&client		= retrieveClient(server, client_fd);
	std::string	nickname	= _clients[i - 1].nickname;
	std::string	username	= _clients[i - 1].username;

	if (command_split[0] == " ")
		command_split[0].erase(0, 1);
	command_split[0].insert(0, ":");
	SendMessage(_clients[i].fd,  RPL_PONG(user_id(nickname, username), command_split[0]));
	// addToClientBuffer(server, _clients[i - 1].fd, RPL_PONG(user_id(nickname, username), cmd[0]));

	return (1);
}

	//? WHOIS ####################################
void Server::whois(int i, std::vector<std::string> command_split){
	(void) i;
	for (int index = 0; index < _nb_client - 1; index++){
		if (_clients[index].nickname == command_split[1]){
			std::cout << "name: "<<_clients[i].name << "\n";
			std::cout << "hostname: "<<_clients[i].hostname << "\n";
			std::cout << "nickname: "<<_clients[i].nickname << "\n";
			std::cout << "chanel: " <<_clients[i].channel << "\n";
			return;
		}
	}
	std::cout << command_split[1] <<" this client doesn't exist\n";
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
		if (command_split[0] == "PASS")
			pass(i, command_split);
		if (_clients[i - 1].is_registered == true)
		{
			if (command_split[0] == "NICK")
				nick(i, command_split);
			else if (command_split[0] == "USER")
				user(i, command_split);
			else if (command_split[0] == "JOIN")
				join(i, command_split);
			else if (command_split[0] == "PRIVMSG")
				privmsg(i, command_split);
			else if (command_split[0] == "PING")
				ping_cmd(i, command_split);
			else if (command_split[0] == "QUIT")
					client_disconnected(i);
			else if (command_split[0] == "WHOIS")
				whois(i, command_split);
		}
	}
	return (0);
}

//! ############### END MAKE_COMMAND ####################