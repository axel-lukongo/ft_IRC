/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngobert <ngobert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 16:01:06 by ngobert           #+#    #+#             */
/*   Updated: 2023/03/27 16:01:18 by ngobert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string deleteFlags(std::string str)
{
	std::string tmp;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '\r' || str[i] == '\n')
			continue;
		tmp += str[i];
	}
	return (tmp);
}