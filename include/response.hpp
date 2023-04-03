/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngobert <ngobert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:27:00 by ngobert           #+#    #+#             */
/*   Updated: 2023/04/03 13:32:21 by ngobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP

# define RESPONSE_HPP

# define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")

// PASS
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect.\r\n")

// NICK 
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")
# define ERR_NICKNAMEINUSE(client, nickname) (":localhost 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")

// USER
# define RPL_YOURHOST(client, server) (":localhost 002 " + client + " :Your host is " + server + ", running version 0.1\r\n")
# define RPL_CREATED(client, server) (":localhost 003 " + client + " :This server was created " + server + "\r\n")
# define RPL_MYINFO(client, server, version, available) (":localhost 004 " + client + " " + server + " " + version + " " + available + "\r\n")

// JOIN
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " " + channel + " :" + topic + "\r\n")
# define RPL_NAMREPLY(client, channel, nickname) (":localhost 353 " + client + " = " + channel + " :" + nickname + "\r\n")
# define RPL_ENDOFNAMES(client, channel, message) (":localhost 366 " + client + " " + channel + " :" + message + "\r\n")

// PRIVMSG
# define RPL_PRIVMSG(client, channel, message) (":" + client + "!" + client + "@localhost PRIVMSG " + channel + " " + message + "\r\n")

// USER
# define ERR_ALREADYREGISTRED(client) (":localhost 462 " + client + " :You may not reregister.\r\n")
//PONG
# define RPL_PONG(user_id, token) (user_id + " PONG " + token + "\r\n")
// #define PONG(user) (":" + user->getHost() + " PONG :" + user->getNick() + "\r\n")
#define ERR_NEEDMOREPARAMS(user, command) (":" + (user)->getName() + " 461 " + (user)->getNick() + " " + (command) + " :Not enough parameters\r\n")

	# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")

#endif
