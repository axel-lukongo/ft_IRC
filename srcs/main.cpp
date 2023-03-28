
#include "../include/server.hpp"

bool server_run = true;

void signal_sigint(int sig){
	(void) sig;
	std::cout << "--------good bye---------\n";
	server_run = false;
}

int main(int argc, char const **argv)
{
	signal(SIGINT, signal_sigint);
	if(argc != 2) //later we have to change by if(argc != 3)
		std::cout << "!!server_IRC <port>!!\n";
	else
		Server Server(argv[1]);
}