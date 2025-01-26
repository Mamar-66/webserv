/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/26 17:03:47 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"

void routine_servor(std::vector<serveur> &servor)
{
	try
	{
		std::vector<serveur>::iterator it = servor.begin();

		while (true)
		{
			if (poll(&it->pfd, 1, -1) && it->pfd.revents == POLLIN)
			{
				client *new_client = creat_client(it->getFD());
				if (!new_client)
				{
					std::cerr << RED "error creat whit new client" RESET << std::endl;
					continue;
				}
				std::cout << new_client->input << std::endl;
				//delete new_client;
			}
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
