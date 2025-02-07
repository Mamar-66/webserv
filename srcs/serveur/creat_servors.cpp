/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_servors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:10:10 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/07 17:36:23 by omfelk           ###   ########.fr       */
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
		std::cerr << e.what() << RESET << std::endl;
		return NULL;
	}

	return servor;
}

std::vector<serveur> creat_servor(std::vector<std::string> &cut_str_serv)
{
	std::vector<serveur> 	servors;
	serveur 				*servor;

	std::vector<std::string>::iterator it = cut_str_serv.begin();

	int size = cut_str_serv.size();

	std::cout << size << std::endl;

	for (it = cut_str_serv.begin(); it != cut_str_serv.end(); ++it)
	{
		if (size-- <= 0)
			break;
		servor = malloc_serv(*it);
		if (servor == NULL)
			std::cout << "NULL" << std::endl;
		else
			servors.push_back(*servor);
	}

	return servors;
}
