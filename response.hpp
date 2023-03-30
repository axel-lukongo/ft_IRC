/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngobert <ngobert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:27:00 by ngobert           #+#    #+#             */
/*   Updated: 2023/03/29 15:41:29 by ngobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP

# define RESPONSE_HPP

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")

// PASS
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect.\r\n")

// NICK 
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")

// USER
# define RPL_YOURHOST(client, server) (":localhost 002 " + client + " :Your host is " + server + ", running version 0.1\r\n")
# define RPL_CREATED(client, server) (":localhost 003 " + client + " :This server was created " + server + "\r\n")
# define RPL_MYINFO(client, server, version, available) (":localhost 004 " + client + " " + server + " " + version + " " + available + "\r\n")

#endif
