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

	bool is_registered;
	std::string channel;
};

#endif // !CLIENT_HPP