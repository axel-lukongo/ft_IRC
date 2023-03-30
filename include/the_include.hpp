#ifndef THE_INCLUDE
#define THE_INCLUDE
# include <exception>
#include "../include/the_include.hpp"
// #include <iostream>
// #include <string>
#include "client.hpp"
#include "response.hpp"
// #include <sstream>
// #include <algorithm>
// #include <csignal>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <csignal>

#define dessin "_____________________________ \n /\\                            \\\n/++\\   _________________________\\\n\\+++\\  \\************************/ \n \\+++\\  \\___________________ **/ \n  \\+++\\  \\   good bye /+++/***/ \n   \\+++\\  \\          /+++/***/ \n    \\+++\\  \\        /+++/***/ \n     \\+++\\  \\      /+++/***/ \n      \\+++\\  \\    /+++/***/ \n       \\+++\\  \\  /+++/***/ \n        \\+++\\  \\/+++/***/ \n         \\+++\\ /+++/***/\n          \\+++++++/***/ \n           \\+++++/***/ \n            \\+++/***/ \n             \\+/___/"

struct my_execption: public std::exception
{
	virtual const char* what(void) const throw(){
		return dessin;
	}
};


int	ping_cmd(Server *server, int const client_fd, cmd_struct &cmd);


#endif // !THE_INCLUDE
