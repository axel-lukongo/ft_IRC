#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include "iostream"
#include <vector>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "../include/server.hpp"


int main(int argc, char const **argv)
{
	if(argc != 2) //later we have to change by if(argc != 3)
		std::cout << "!!server_IRC <port>!!\n";
	else
		Server Server(argv[1]);
}