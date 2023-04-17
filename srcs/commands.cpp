/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngobert <ngobert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:09:59 by ngobert           #+#    #+#             */
/*   Updated: 2023/04/12 16:18:54 by ngobert          ###   ########.fr       */
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
		_clients[i - 1].is_connected = true;
		SendMessage(_clients[i - 1].fd, RPL_WELCOME(_clients[i - 1].nickname, _clients[i - 1].nickname));
	}
	else
		SendMessage(_clients[i - 1].fd, ERR_PASSWDMISMATCH(_clients[i - 1].nickname));
}










	//? NICK ####################################

int		Server::is_nickname_used(std::string nickname, int i)
{
	size_t x = i;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		if (j == x - 1)
			continue;
		if (_clients[j].nickname == nickname)
			return (1);
	}
	return (0);
}

void	Server::nick(int i, std::vector<std::string> command_split)
{
	int c = 0;
	std::string tmp = command_split[1];
	while (is_nickname_used(command_split[1], i))
	{
		command_split[1] += "_";
		c++;
	}
	_clients[i - 1].old_nickname = _clients[i - 1].nickname;
	_clients[i - 1].nickname = command_split[1];
	if (c != 0)
	{
		SendMessage(_clients[i - 1].fd, ERR_NICKNAMEINUSE(tmp, tmp));
		SendMessage(_clients[i - 1].fd, RPL_NICK(_clients[i - 1].old_nickname, _clients[i - 1].nickname, _clients[i - 1].nickname));
	}
	else
		SendMessage(_clients[i - 1].fd, RPL_NICK(_clients[i - 1].old_nickname, _clients[i - 1].nickname, _clients[i - 1].nickname));
}




	//? USER ####################################
void	Server::user(int i, std::vector<std::string> command_split)
{
	if (_clients[i - 1].is_registered)
	{
		SendMessage(_clients[i - 1].fd, ERR_ALREADYREGISTRED(_clients[i - 1].nickname));
		return ;
	}
	_clients[i - 1].username = command_split[1];
	_clients[i - 1].hostname = command_split[2];
	_clients[i - 1].servername = command_split[3];
	_clients[i - 1].realname = command_split[4];

	
	SendMessage(_clients[i - 1].fd, RPL_YOURHOST(_clients[i - 1].nickname, _clients[i - 1].servername));
	SendMessage(_clients[i - 1].fd, RPL_CREATED(_clients[i - 1].nickname, _clients[i - 1].servername));
	SendMessage(_clients[i - 1].fd, RPL_MYINFO(_clients[i - 1].nickname, _clients[i - 1].servername, "0.0.1", "0.0.1"));
	_clients[i - 1].is_registered = true;
}










	//? JOIN ####################################
/**
 * @brief in this function i will add the user in the class _channel
 * 
 * @param i the index of the user
 * @param chanel_exist 
 * @param command_split it the command send by the client
 */
void Server::join_the_channel(int i,bool chanel_exist, std::vector<std::string> command_split){
	Channel my_chanel;
	if(chanel_exist == false){//if the channel don't exist yet
		command_split[1].erase(0,1);
		my_chanel.name = command_split[1]; //it for add the name of the channels
		my_chanel.operators.push_back(_clients[i - 1].nickname); // it for add the operator
		_channels.push_back(my_chanel); //i add in the list of channels
	}
	for (size_t j = 0; j < _channels.size(); j++){ //this loop it for add the user in the user in a channel
		if (_channels[j].name == command_split[1]){
			_channels[j].users.push_back(_clients[i - 1].nickname);
		}
	}

}





//########################





/**
 * @brief this function it for check if the channel already exist
 * 
 * @param i 
 * @param command_split it the command send by the client
 * command_split[1]: it the name of the chanel that i want join
 * @return true it mean the channel exist
 * @return false it mean the channel does not exist
 */
bool Server::chanel_is_exist(int i, std::string command_split){
	(void) i;
		_clients[i - 1].channel = command_split;
	for(size_t index = 0; index < _channels.size(); index++){ //in this loop i check if the channel already exist
		if (_channels[index].name == command_split)
				return true;
	}
	return false;
}

//########################


/**
 * @brief this fonction it for check if the user has been banned
 * 
 * @param i the index of the client
 */

bool Server::is_banned(int i, std::string channel_name){
	for (size_t j = 0; j < _channels.size(); j++){ // i go trought all channels
			if(_channels[j].name == channel_name){ //if i find the channel who i looking for
				std::vector<std::string>::iterator it = std::find(_channels[j].banned_users.begin(), _channels[j].banned_users.end(), _clients[i -1].nickname);
				if(it != _channels[j].banned_users.end()) // i look in this channel if the curent user is not banned
					return true;
			}
		}
	return false;
}


void	Server::join(int i, std::vector<std::string> command_split)
{
	std::string channel_name = command_split[1];
	channel_name.erase(0,1); //it just for erase the "#" because it creat some for my comparaisons
	bool chanel_exist = chanel_is_exist(i, channel_name); //a bool for verify if the chanel already exist

	if(chanel_exist == true){
		if (is_banned(i, channel_name) == true){
			SendMessage(_clients[i - 1].fd, ERR_BANNEDFROMCHAN(_clients[i - 1].nickname, _clients[i - 1].channel)); //if he is ban i send a msg befor to return
			return;
		}
		
	}

	join_the_channel(i, chanel_exist, command_split);//this is where i going join the chanel
	_clients[i - 1].channels_joined.push_back(_clients[i - 1].channel);//this is the channel where my client is
	SendMessage(_clients[i - 1].fd, RPL_TOPIC(_clients[i - 1].nickname, _clients[i - 1].channel, "No topic is set"));
	SendMessage(_clients[i - 1].fd, RPL_NAMREPLY(_clients[i - 1].nickname, _clients[i - 1].channel, _clients[i - 1].nickname));
	SendMessage(_clients[i - 1].fd, RPL_ENDOFNAMES(_clients[i - 1].nickname, _clients[i - 1].channel, "End of /NAMES list"));
	_clients[i - 1].channel = channel_name;
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
			{
				SendMessage(_clients[j].fd, RPL_PRIVMSG(nickname, channel, tmp));
				return ;
			}
		}
		SendMessage(_clients[i - 1].fd, ERR_NOSUCHNICK(nickname, channel));
	}
	else // send a message to another client
	{
		for (size_t j = 0; j < _clients.size(); j++)
		{
			if (_clients[j].nickname == channel && _clients[j].nickname != nickname)
			{
				SendMessage(_clients[j].fd, RPL_PRIVMSG(nickname, channel, tmp));
				return ;
			}
		}
		SendMessage(_clients[i - 1].fd, ERR_NOSUCHNICK(nickname, channel));

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
			std::cout << "name: "<<_clients[index].name << "\n";
			std::cout << "hostname: "<<_clients[index].hostname << "\n";
			std::cout << "nickname: "<<_clients[index].nickname << "\n";
			std::cout << "chanel: " <<_clients[index].channel << "\n";
			return;
		}
	}
	std::cout << command_split[1] <<" this client doesn't exist\n";
}









void Server::mode(int i, std::vector<std::string> command_split){
	std::map<std::string, std::string>::iterator it = _clients[i - 1].mode.find(command_split[1]); 
	bool client_exist = false;
	if(it->second == "+o"){
		size_t j = 0;
		for(; j < _clients.size(); j++){
			if (_clients[j].nickname == command_split[3]){
				if (command_split[2] == "+b")
					_clients[j].mode[command_split[1]] = command_split[2];
				client_exist = true;
				break;
			}
		}
		if(client_exist == false){
			std::cout << "this client doesn't exist";
			return;
		}
	}
	else
		std::cout << "\n\n\n=============you are not a operator==============\n\n\n ";
}

	//? KICK ####################################
void Server::kick(int i, std::vector<std::string> command_split)
{
	std::string banner = _clients[i - 1].nickname;
	std::string channel = command_split[1];
	std::string to_ban = command_split[2];
	int	x = 0;
	// take the rest of the arguments and put it in the message
	std::string tmp;
	for (size_t j = 3; j < command_split.size(); j++)
	{
		tmp += command_split[j];
		tmp += " ";
	}
	// Kick command if the user is a operator
	if (std::find(_clients[i - 1].channels_operated.begin(), _clients[i - 1].channels_operated.end(), channel) != _clients[i - 1].channels_operated.end())
	{
		std::cout << CYAN << "YOU OPERATOR" << RESET << std::endl;
		for (size_t j = 0; j < _clients.size(); j++)
		{
			if (_clients[j].nickname == to_ban)
			{
				SendMessage(_clients[j].fd, RPL_KICK(banner, _clients[i - 1].username,channel, to_ban, tmp));
				_clients[j].channel = "";
				x = 1;
				break;
			}
		}
		if (x == 0)
			SendMessage(_clients[i - 1].fd, ERR_NOSUCHNICK(banner, to_ban));
	}
}


	//? NOTICE ####################################
void Server::notice(int i, std::vector<std::string> command_split)
{
	std::string channel = command_split[1];
	std::string nickname = _clients[i - 1].nickname;
	std::string username = _clients[i - 1].username;

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
				SendMessage(_clients[j].fd, RPL_NOTICE(nickname, username,channel, tmp));
		}
	}
	else // send a message to another client
	{
		for (size_t j = 0; j < _clients.size(); j++)
		{
			if (_clients[j].nickname == channel && _clients[j].nickname != nickname)
				SendMessage(_clients[j].fd, RPL_NOTICE(nickname, username,channel, tmp));
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
		if (command_split[0] == "PASS")
			pass(i, command_split);
		if (_clients[i - 1].is_connected == true)
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
			else if (command_split[0] == "WHOIS")
				whois(i, command_split);
			else if (command_split[0] == "MODE")
				mode(i, command_split);
			else if (command_split[0] == "NOTICE")
				notice(i, command_split);
			else if (command_split[0] == "KICK")
				kick(i, command_split);
			// else if (command_split[0] == "QUIT")
			// 		client_disconnected(i);
		}
	}
	return (0);
}

//! ############### END MAKE_COMMAND ####################