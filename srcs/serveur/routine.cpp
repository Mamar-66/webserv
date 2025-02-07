/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/07 11:32:51 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"


void	monitoring(serveur &servor, char **env)
{
	std::cout << ORANGE "server listen . . ." RESET << std::endl;

	for (size_t i = 0; i < servor.all_pollfd.size(); ++i)
	{
		servor.all_pollfd[i].revents = 0;
		int readpoll = poll(servor.all_pollfd.data(), servor.all_pollfd.size(), -1);
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
		else if(servor.all_pollfd[i].fd != servor.getFD() && servor.all_pollfd[i].revents & POLLOUT)
		{
			try
			{
				responding(servor, servor.all_pollfd[i].fd);
			}
			catch(const std::exception& e)
			{
				throw;
			}
		}
		else if (servor.all_pollfd[i].revents & POLLERR || servor.all_pollfd[i].revents & POLLHUP || servor.all_pollfd[i].revents & POLLNVAL)
    	{
        	std::cout << "Client FD " << servor.all_pollfd[i].fd << " déconnecté.\n";
        	close(servor.all_pollfd[i].fd);
			throw std::runtime_error("ee");
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
