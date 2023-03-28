#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string.h>
#include "iostream"

struct Client {
    int fd;
    std::string name;
	std::string old_name;

	std::string nickname;
	bool is_registered = false;
	std::string channel;
};

#endif // !CLIENT_HPP