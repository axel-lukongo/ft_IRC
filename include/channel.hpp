#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string.h>
#include "iostream"

struct Channel 
{
	std::string					name;
	std::string					topic;
	std::vector<std::string>	users;
	std::vector<std::string>	operators;
	std::vector<std::string>	banned_users;
	// std::vector<std::string>	invited;
	std::vector<std::string>	voice;
};

#endif // !CHANNEL_HPP