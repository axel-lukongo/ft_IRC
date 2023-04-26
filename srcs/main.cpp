
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

bool check_if_port_valid(std::string port)
{
	int i = 0;
	while (port[i])
	{
		if (!isdigit(port[i]))
			return false;
		i++;
	}
	int port_int = atoi(port.c_str());
	if (port_int < 1024 || port_int > 65535)
		return false;
	return true;
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
		{
			if (!check_if_port_valid(argv[1]))
			{
				std::cout << "!!port invalid!!\n";
				return (1);
			}
			server.run(argv[1], argv[2]);
		}
			// Server * server = Server(argv[1], argv[2]);
	}
	catch (const my_execption & e)
	{
		std::cout << e.what() << std::endl;
		
		// return (1);
	}
}