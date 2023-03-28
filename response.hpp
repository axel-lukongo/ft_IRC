/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngobert <ngobert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:27:00 by ngobert           #+#    #+#             */
/*   Updated: 2023/03/28 20:41:12 by ngobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP

# define RESPONSE_HPP

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")

// PASS
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect.\r\n")

// NICK 
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")

#endif
