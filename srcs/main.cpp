
#include "../include/server.hpp"
#include "../include/the_include.hpp"

bool server_run = true;

void signal_sigint(int sig){
	if(sig == SIGINT)
		throw my_execption();
	else if (sig == SIGPIPE)
		return;
	// server_run = false;
}

int main(int argc, char const **argv)
{
	Server server;
	try{
		signal(SIGINT, signal_sigint);
		signal(SIGPIPE, signal_sigint);

		if(argc != 3) //later we have to change by if(argc != 3)
			std::cout << "!!server_IRC <port>!!\n";
		else
			server.run(argv[1], argv[2]);
			// Server * server = Server(argv[1], argv[2]);
	}
	catch (const my_execption & e)
	{
		std::cout << e.what() << std::endl;
		
		// return (1);
	}
}