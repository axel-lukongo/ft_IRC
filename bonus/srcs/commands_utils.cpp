#include "../include/server.hpp"
#include "../include/the_include.hpp"



/**
 * @brief in this function i will add the user in the class _channel
 * 
 * @param i the index of the user
 * @param chanel_exist 
 * @param command_split it the command send by the client
 */
void Server::join_the_channel(int i,bool chanel_exist, std::vector<std::string> command_split){
	Channel my_chanel;
		command_split[1].erase(0,1);
	if(chanel_exist == false){//if the channel don't exist yet
		my_chanel.name = command_split[1]; //it for add the name of the channels
		my_chanel.operators.push_back(_clients[i - 1].nickname); // it for add the operator
		my_chanel.invite_flag = false;
		_channels.push_back(my_chanel); //i add in the list of channels
	}

	for (size_t j = 0; j < _channels.size(); j++){ //this loop it for add the user in the user in a channel
		if (_channels[j].name == command_split[1]){
			_channels[j].users.push_back(_clients[i - 1].nickname);
		}
	}

}



//################################################################################



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
	(void)i;
	_clients[i - 1].channel = command_split;

	for(size_t index = 0; index < _channels.size(); index++){ //in this loop i check if the channel already exist
		if (_channels[index].name == command_split)
				return true;
	}
	return false;
}



//################################################################################



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



//################################################################################



std::string Server::topic_exist(std::string channel_name){
	std::string tmp_channel_name = channel_name;
	tmp_channel_name = tmp_channel_name.erase(0,1);
	for(size_t j = 0; j < _channels.size(); j++){
		if(_channels[j].name == tmp_channel_name){
			if(!_channels[j].topic.empty())
				return _channels[j].topic;
		}
	}
	return "";
}




//################################################################################



Channel* Server::find_channels(std::string channel_name){
	size_t j = 0;
	for(; j < _channels.size(); j++){
		if(_channels[j].name == channel_name){
			
			return &_channels[j]; //if i find the channell i return this
		}
	}
	return &_channels[j]; // i still return _channels[j] because j has been increase until _channels.size()
}



//################################################################################




bool Server::client_is_invited(std::string client_name ,std::string channel_name){
	Channel *channel_tmp = find_channels(channel_name);

	if(channel_tmp->invite_flag == true){
	// std::cout << " =========== ::  :: =============\n\n";
		std::vector<std::string>::iterator it = std::find(channel_tmp->invited.begin(),channel_tmp->invited.end(), client_name);
		if(it == channel_tmp->invited.end()){
			return false;
		}
	}
	return true;
}




//################################################################################




void Server::share_msg_chan(const std::string info, std::string channel_name) {
	(void) channel_name;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		if (_clients[j].channel == ("#"+channel_name))
			send(_clients[j].fd, info.c_str(), info.size(), MSG_NOSIGNAL);
	}
}



//################################################################################



void Server::share_msg_all(const std::string info) {
	for (size_t j = 0; j < _clients.size(); j++)
	{
		send(_clients[j].fd, info.c_str(), info.size(), MSG_NOSIGNAL);
	}
}



//################################################################################




bool Server::is_operator(std::string client_name, std::string channel_name){
	std::vector<std::string>::iterator it;
	for (size_t j = 0; j < _channels.size(); j++){ //this loop it for look if my user is a operators
		if (_channels[j].name == channel_name){
			it = std::find(_channels[j].operators.begin(), _channels[j].operators.end(), client_name);
			if(it == _channels[j].operators.end()){
				std::cout << "\n\n======= you are not a operators  =========\n\n";
				return false;
			}
		}
	}
	return true;
}



//################################################################################




void Server::share_topic(std::string channel_name, std::string msg){
	// (void) channel_name;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		// std::cout << " ========= "<<_clients[j].channel << " ========== " << channel_name << " ===========\n\n";
		if (_clients[j].channel == ("#"+channel_name))
			send(_clients[j].fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
	}
}



//################################################################################



void Server::mode_for_user(std::vector<std::string> command_split, std::string channel_name){
	for(size_t j = 0; j < _clients.size(); j++){ // i go throught all client
				if (_clients[j].nickname == command_split[3]){ // i look if the client exist
					if (command_split[2] == "+b"){ // i will add amoung the banned user
					Channel * tmp_channel = find_channels(channel_name);
					tmp_channel->banned_users.push_back(command_split[3]);
					}
					else if (command_split[2] == "-b")
					{
						Channel * tmp_channel = find_channels(channel_name);
						std::vector<std::string>::iterator it = std::find(tmp_channel->banned_users.begin(),tmp_channel->banned_users.end(), _clients[j].nickname);
						tmp_channel->banned_users.erase(it);
					}
					else if (command_split[2] == "+o"){ // i will add amoung the operators
					Channel * tmp_channel = find_channels(channel_name);
					tmp_channel->operators.push_back(command_split[3]);
					}
					else
						std::cout << command_split[2] << " invalid option";
					return;
				}
			}
}




//################################################################################




void Server::mode_for_channels(int i, std::vector<std::string> command_split, std::string channel_name){
			// std::cout << " =========== ::  :: =============\n\n";
	if (command_split[2] == "+i"){ 
				Channel *tmp_channel = find_channels(channel_name);
				tmp_channel->invite_flag = true;
				std::string info = ":" + _clients[i - 1].getName() + " sets mode +i " + channel_name + "\r\n";
				share_msg_chan(info, channel_name);
			}
			else if (command_split[2] == "-i"){
				Channel *tmp_channel = find_channels(channel_name);
				tmp_channel->invite_flag = false;
				std::string info = ":" + _clients[i - 1].getName() + " sets mode -i " + channel_name + "\r\n";
				share_msg_chan(info, channel_name);
			}
}
