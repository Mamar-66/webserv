/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/06 14:51:54 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"


void	monitoring(serveur &servor, char **env)
{
	std::cout << ORANGE "server listen . . ." RESET << std::endl;

	size_t size = servor.all_pollfd.size();
	for (size_t i = 0; i < size; ++i)
	{
		int readpoll = poll(servor.all_pollfd.data(), size, -1);
		if (readpoll < 0)
			throw std::runtime_error(RED "Error poll = -1");

		if (servor.all_pollfd[i].fd == servor.getFD() && servor.all_pollfd[i].revents & POLLIN)
		{
			try
			{
				creat_client(servor, env);
			}
			catch(const std::exception& e)
			{
				throw;
			}
		}

		if (servor.all_pollfd[i].fd != servor.getFD() && servor.all_pollfd[i].revents & POLLOUT)
		{
			std::cout << GREEN << "simon" << RESET << std::endl;

			try
			{
				responding(servor, servor.all_pollfd[i].fd);
			}
			catch(const std::exception& e)
			{
				throw;
			}
		}
	}
}

void	routine_servor(std::vector<serveur> &servor, char **env)
{
	try
	{
		std::vector<serveur>::iterator itserv = servor.begin();

		while (true)
		{
			monitoring(*itserv, env);
			std::cout << BLUE "iiiiiiiiiiiiiiiiiiiiiiiiiiiii" RESET << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
