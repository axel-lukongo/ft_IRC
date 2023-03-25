#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string.h>
#include "iostream"

struct Client {
    int fd;
    std::string name;
};

#endif // !CLIENT_HPP