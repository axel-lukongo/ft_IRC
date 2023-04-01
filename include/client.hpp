#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string.h>
#include "iostream"
#include <map>
struct Client {
    int fd;
	std::string nickname;

    std::string name;
	std::string old_name;
	std::string old_nickname;

	std::string username;
	std::string hostname;
	std::string servername;
	std::string realname;

	bool is_registered;
	std::string channel;

	std::vector<std::string> channels_joined;
	std::vector<std::string> channels_operated;
	std::map<std::string,std::string> mode;//this variable it for know the right of the user in the chanel
	std::vector<std::string> channels_created;
	std::vector<std::string> channels_banned;
	// public:
	// 	std::string get_nickname(){
	// 		return nickname;
	// 	}
	// 	std::string get_host(){
	// 		return hostname;
	// 	}
	// 	int getFD(){
	// 		return fd;
	// 	}

};

#endif // !CLIENT_HPP