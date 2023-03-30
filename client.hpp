#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string.h>
#include "iostream"

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
	std::vector<std::string> channels_created;
	std::vector<std::string> channels_banned;
};

#endif // !CLIENT_HPP