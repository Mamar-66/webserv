/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_servors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:10:10 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/08 11:56:52 by omfelk           ###   ########.fr       */
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

std::vector<serveur*> creat_servor(std::vector<std::string> &cut_str_serv)
{
	int size = cut_str_serv.size();
	int i = 0;

	if (size <= 0)
		throw std::runtime_error("Error no compliant server");

	std::vector<serveur*> 	servors;
	serveur 				*servor;

	servors.resize(size, NULL);

	for (std::vector<std::string>::iterator it = cut_str_serv.begin(); it != cut_str_serv.end(); ++it)
	{
		if (i >= size)
			break;

		servor = malloc_serv(*it);
		if (servor != NULL)
			servors[i++] = servor;
	}

	for (; size > i; --size)
		servors.erase(servors.begin() + i);

	return servors;
}
