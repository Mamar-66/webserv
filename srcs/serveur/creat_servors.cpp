/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_servors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:10:10 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/23 11:06:13 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"

serveur *malloc_serv(std::string &str)
{
	serveur *servor = NULL;

	std::cout << str << std::endl;

	try
	{
		servor = new serveur(str);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return NULL;
	}

	return servor;
}

void	creat_servor(std::vector<std::string> &cut_str_serv, monitoring &moni)
{
	int size = cut_str_serv.size();
	int i = 0;

	if (size <= 0)
		throw std::runtime_error("Error no compliant server");

	serveur 				*servor;

	for (std::vector<std::string>::iterator it = cut_str_serv.begin(); it != cut_str_serv.end(); ++it)
	{
		if (i >= size)
			break;

		servor = malloc_serv(*it);
		if (servor != NULL)
		{
			for (std::vector<pollfd>::iterator it = servor->all_pollfd.begin(); it != servor->all_pollfd.end(); ++it)
			{
				moni.servors[it->fd] = servor;
				moni.all_all_pollfd.push_back(*it);
				moni.all_pollfd_servor.push_back(*it);
			}
		}
	}
}
