
#include "../include/server.hpp"
#include "../include/the_include.hpp"


void Server::bot(int i, std::string msg){
	if(msg == ":salut  "){
		std::string respons = ":bot!bot@localhost PRIVMSG " + _clients[i - 1].nickname + " :salut ca va ? .\n";
		send(_clients[i - 1].fd, respons.c_str(), respons.size(),MSG_NOSIGNAL);
	}
	else if (msg == ":qui es tu ?  "){
		std::string respons = ":bot!bot@localhost PRIVMSG " + _clients[i - 1].nickname + " :je suis max, je suis un bot cree par axel et nathan pour valider le projet a 125/125\n";
		send(_clients[i - 1].fd, respons.c_str(), respons.size(),MSG_NOSIGNAL);

	}
	else if (msg == ":que peux tu faire ?  "){
		std::string respons = ":bot!bot@localhost PRIVMSG " + _clients[i - 1].nickname + " :je ne peux rien faire mdr, a part faire semblant de communiquer avec toi\n";
		send(_clients[i - 1].fd, respons.c_str(), respons.size(),MSG_NOSIGNAL);
	}
	else{
		std::string respons = ":bot!bot@localhost PRIVMSG " + _clients[i - 1].nickname + " :ah d'accord c'est chouette (je n'ai absolu;ent rien compris)\n";
		send(_clients[i - 1].fd, respons.c_str(), respons.size(),MSG_NOSIGNAL);
	}
}