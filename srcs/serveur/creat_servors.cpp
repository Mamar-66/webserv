/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_servors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:10:10 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/24 12:05:14 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"

serveur *malloc_serv(std::string &str)
{
	serveur *server = new serveur(str);
	if (!server)
		throw std::runtime_error("Error creat servor");

	return server;
}

std::vector<serveur> creat_servor(std::vector<std::string> &cut_str_serv)
{
	std::vector<serveur> 	servors;
	// serveur					*server;

	std::vector<std::string>::iterator it;
	int i = 0;

	try
	{
		for (it = cut_str_serv.begin(); it != cut_str_serv.end(); ++it)
		{
			std::cout << *it << "i = " << i++ << std::endl;
			// server = ;

			servors.push_back(*malloc_serv(*it));
		}
	}
	catch(const std::exception& e)
	{
		throw;
	}

	std::cout << "ok" << std::endl;

	return servors;
}
