#ifndef THE_INCLUDE
#define THE_INCLUDE
# include <exception>

#define dessin "_____________________________ \n /\\                            \\\n/++\\   _________________________\\\n\\+++\\  \\************************/ \n \\+++\\  \\___________________ **/ \n  \\+++\\  \\   good bye /+++/***/ \n   \\+++\\  \\          /+++/***/ \n    \\+++\\  \\        /+++/***/ \n     \\+++\\  \\      /+++/***/ \n      \\+++\\  \\    /+++/***/ \n       \\+++\\  \\  /+++/***/ \n        \\+++\\  \\/+++/***/ \n         \\+++\\ /+++/***/\n          \\+++++++/***/ \n           \\+++++/***/ \n            \\+++/***/ \n             \\+/___/"

struct my_execption: public std::exception
{
	virtual const char* what(void) const throw(){
		return dessin;
	}
};


#endif // !THE_INCLUDE
