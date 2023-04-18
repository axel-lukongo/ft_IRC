/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 14:09:59 by ngobert           #+#    #+#             */
/*   Updated: 2023/04/18 19:52:45 by alukongo         ###   ########.fr       */
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
	else{
		SendMessage(_clients[i - 1].fd, ERR_PASSWDMISMATCH(_clients[i - 1].nickname));
		_clients[i - 1].is_connected = false;
	}
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
	_clients[i - 1].hostname = "localhost";
	_clients[i - 1].servername = command_split[3];
	_clients[i - 1].realname = command_split[4];
	SendMessage(_clients[i - 1].fd, RPL_YOURHOST(_clients[i - 1].nickname, _clients[i - 1].servername));
	SendMessage(_clients[i - 1].fd, RPL_CREATED(_clients[i - 1].nickname, _clients[i - 1].servername));
	SendMessage(_clients[i - 1].fd, RPL_MYINFO(_clients[i - 1].nickname, _clients[i - 1].servername, "0.0.1", "0.0.1"));
}







//? JOIN ###################################################################################################

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
		if (client_is_invited(_clients[i - 1].nickname, channel_name) == false){ //only if the chan is in invite mode
			std::string msg = ERR_NEEDINVITE(_clients[i - 1], command_split[1]);
			SendMessage(_clients[i - 1].fd, msg); //if he is not invite i send this
			return;
		}
	}

	join_the_channel(i, chanel_exist, command_split);//this is where i going join the chanel
	_clients[i - 1].channels_joined.push_back(command_split[1]);//this is the channel where my client is
	std::string info = ":" + _clients[i - 1].getName() + " JOIN #" + channel_name + "\r\n";
	SendMessage(_clients[i - 1].fd, info);
	std::string topic = topic_exist(i);
	if(topic != "")
		SendMessage(_clients[i - 1].fd, RPL_TOPIC(_clients[i - 1], _clients[i - 1].channel, topic));
	SendMessage(_clients[i - 1].fd, RPL_NAMREPLY(_clients[i - 1].nickname, _clients[i - 1].channel, _clients[i - 1].nickname));
	SendMessage(_clients[i - 1].fd, RPL_ENDOFNAMES(_clients[i - 1].nickname, _clients[i - 1].channel, "End of /NAMES list"));
	_clients[i - 1].channel = command_split[1];
}






//? PRIVMSG #######################################################################################

void	Server::privmsg(int i, std::vector<std::string> command_split)
{
	std::string channel = command_split[1];
	std::string nickname = _clients[i - 1].nickname;

	//take the rest of the arguments and put it in the message
	std::string msg;
	for (size_t j = 2; j < command_split.size(); j++)
	{
		msg += command_split[j];
		msg += " ";
	}
	if (command_split[1][0] == '#') // here i look if i send the msg to a chanel
	{
		if(_clients[i - 1].channel == channel){
			for (size_t j = 0; j < _clients.size(); j++)
			{
				if (_clients[j].channel == channel && _clients[j].nickname != nickname)
					SendMessage(_clients[j].fd, RPL_PRIVMSG(nickname, channel, msg));
			}
		}
	}
	else // send a message to another client
	{
		for (size_t j = 0; j < _clients.size(); j++)
		{
			if (_clients[j].nickname == command_split[1])
				SendMessage(_clients[j].fd, RPL_PRIVMSG(nickname, _clients[j].nickname , msg));
		}
	}
}







//? PING ########################################################################################

void	Server::ping_cmd(int i, std::vector<std::string> command_split)
{
	// Client		&client		= retrieveClient(server, client_fd);
	std::string	nickname	= _clients[i - 1].nickname;
	std::string	username	= _clients[i - 1].username;

	if (command_split[0] == " ")
		command_split[0].erase(0, 1);
	command_split[0].insert(0, ":");
	SendMessage(_clients[i].fd,  RPL_PONG(user_id(nickname, username), command_split[0]));

	// return (1);
}






//? WHOIS #########################################################################################

void Server::whois(int i, std::vector<std::string> command_split){
	(void) i;
	for (size_t index = 0; index < _clients.size(); index++){
		if (_clients[index].nickname == command_split[1]){
			SendMessage(_clients[i - 1].fd, (RPL_WHOISUSER(_clients[i - 1], _clients[index])));
			std::cout << "username: "<<_clients[index].username << "\n";
			std::cout << "hostname: "<<_clients[index].hostname << "\n";
			std::cout << "nickname: "<<_clients[index].nickname << "\n";
			std::cout << "chanel: " <<_clients[index].channel << "\n";
			return;
		}
	}
	SendMessage(_fds[0].fd, ERR_NOSUCHNICK(_clients[i - 1].nickname, command_split[1]));
}





//? MODE #######################################################################################

void Server::mode(int i, std::vector<std::string> command_split){
	std::string channel_name = command_split[1];
	if (is_operator(_clients[i - 1].nickname, channel_name.erase(0,1)) == false)
		return;
	if(chanel_is_exist(i, channel_name) && (command_split.size() == 5 || command_split.size() == 4)){
		if (command_split.size() == 5){
			mode_for_user(command_split, channel_name);
		}
		else if(command_split.size() == 4){
			mode_for_channels(i,command_split, channel_name);
		}
		else
			std::cout << command_split[2] << " invalid option";
		return;
	}
}






//? PART #######################################################################################


void Server::part(int i, std::vector<std::string> command_split){
	(void) command_split;
	std::string chanel_of_client = _clients[i - 1].channel.erase(0,1);
	for (size_t j = 0; j < _channels.size(); j++){
		if (_channels[j].name == chanel_of_client){

//part 1 i remove the user from the channel class
			std::string info = ":" + _clients[i - 1].getName() + " PART #" + chanel_of_client + " " + _clients[i - 1].nickname +"\r\n";


//part 2 i send message to the all user in the channel
			share_msg(info, chanel_of_client);
			send(_clients[i - 1].fd, info.c_str(), info.length(), MSG_NOSIGNAL);
			_channels[j].users.erase(std::remove(_channels[j].users.begin(), _channels[j].users.end(), _clients[i - 1].nickname), _channels[j].users.end());
			if(_clients[i - 1].channels_joined.size() > 1){
				_clients[i -1].channels_joined.pop_back();
				_clients[i - 1].channel = *(_clients[i - 1].channels_joined.end() - 1);
			}
			else
				_clients[i - 1].channel.clear();

//part 3 i check if he was an operator
			std::vector<std::string>::iterator it = std::find(_channels[j].operators.begin(), _channels[j].operators.end(), _clients[i - 1].nickname);
			if(it != _channels[j].operators.end()){
				_channels[j].operators.erase(std::remove(_channels[j].operators.begin(), _channels[j].operators.end(), _clients[i - 1].nickname), _channels[j].operators.end());
			}
			return;
		}
	}
	SendMessage(_clients[i - 1].fd, ERR_NOSUCHCHANNEL(_clients[i - 1], chanel_of_client));
}




//? QUIT #######################################################################################

void Server::quit(int i, std::vector<std::string> command_split){
	// close(_fds[i].fd);
	(void) command_split;
	while (_clients[i - 1].channels_joined.size() > 0){
		part(i, command_split);
	}
	_clients.erase(_clients.begin() + (i - 1));
	// _fds[i].fd = 0;
	std::cout << "Client " << _clients[i-1].nickname << " disconnected \n\n";
}






//? TOPIC #######################################################################################


/**
 * @brief 
 * 
 * @param i 
 * @param command_split[1]: is the name of channel that i want apply the topic;
 * command_split[2]: is the ":";
 * command_split[...]: the topic name
 */

void Server::topic(int i, std::vector<std::string> command_split){
	std::vector<std::string>::iterator it;
	for (size_t j = 0; j < _channels.size(); j++){
			if(_channels[j].name == command_split[1].erase(0,1)){
				if (is_banned(i, _channels[j].name) == true){
				SendMessage(_clients[i - 1].fd, ERR_BANNEDFROMCHAN(_clients[i - 1].nickname, _clients[i - 1].channel)); //if he is ban i send a msg befor to return
				std::string is_banned = ERR_BANNEDFROMCHAN(_clients[i - 1].nickname, _clients[i - 1].channel);
				send(_clients[i - 1].fd, is_banned.c_str(), is_banned.size(), MSG_NOSIGNAL);
				return;
			}

	//step 2: check if the client is a operator, else i send ERR_CHANOPRIVSNEEDED
			if (_channels[j].name == command_split[1]){
				it = std::find(_channels[j].operators.begin(), _channels[j].operators.end(), _clients[i - 1].nickname);
				if(it == _channels[j].operators.end()){
					SendMessage(_clients[i - 1].fd, ERR_CHANOPRIVSNEEDED(_clients[i - 1], _channels[j].name)); //if he is ban i send a msg befor to return
					std::string not_operator = ERR_CHANOPRIVSNEEDED(_clients[i - 1], _channels[j].name);
					send(_clients[i - 1].fd, not_operator.c_str(), not_operator.length(), MSG_NOSIGNAL);
					return;
				}
			}

	//step 3: check if the user try to creat a new topic or just try to see the topic name
			if (command_split[2][0] == ':'){
				//i try to creat a new topic
				std::string my_new_topic = "";
				for (size_t x = 2; x < command_split.size(); x++){
					my_new_topic += command_split[x];
					my_new_topic += " ";
				}
				//ici je cree le nx topic
				_channels[j].topic = my_new_topic;
				share_topic(_channels[j].name, RPL_TOPIC(_clients[i - 1], _channels[j].name, my_new_topic));
			}
		}
	}
}






//? INVITE #######################################################################################

void Server::invite(int i, std::vector<std::string> command_split){
	

	//########part1 i check if the channel exist and if he is the operator of the channels#####
	std::string name_of_client = "";
	if (is_operator(_clients[i - 1].nickname, command_split[2].erase(0,1)) == false){
		return;
	}
	
	//#######part 2 i check if the client to invite exist########
	for (size_t j = 0; j < _clients.size(); j++){
		if(_clients[j].nickname == command_split[1])
			name_of_client = _clients[j].nickname;
	}
	if(name_of_client == ""){
		std::cout << "client doesn't exist";
		return;
	}

	//########part3 if every thing exist i add him in the invite list#######
	Channel * tmp_channel = find_channels(command_split[2]);
	tmp_channel->invited.push_back(name_of_client);
}


// void Server::init_map(){
// _my_map["NICK"] = &Server::nick;
// _my_map["USER"] = &Server::user;
// _my_map["JOIN"] = &Server::join;
// _my_map["PRIVMSG"] = &Server::privmsg;
// _my_map["PING"] = &Server::ping_cmd;
// _my_map["WHOIS"] = &Server::whois;
// _my_map["MODE"] = &Server::mode;
// _my_map["PART"] = &Server::part;
// _my_map["TOPIC"] = &Server::topic;
// _my_map["INVITE"] =	 &Server::invite;
// _my_map["QUIT"] = 	&Server::quit;

// }


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
		if (command_split[0] == "PASS")
			pass(i, command_split);
		if (_clients[i - 1].is_connected == true)
		{
				std::map<std::string, my_functions>::iterator it;
			it = _my_map.find(command_split[0]);
			if(it != _my_map.end())
				(this->*it->second)(i, command_split);
		}
	}
	return (0);
}

//! ############### END MAKE_COMMAND ####################



//############################## je dois faire topic ##############################