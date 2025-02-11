/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/11 10:49:23 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"


void	monitoring(serveur &servor, char **env)
{
	std::cout << ORANGE "server listen . . ." RESET << std::endl;

	for (size_t i = 0; i < servor.all_pollfd.size(); ++i)
	{
		servor.all_pollfd[i].revents = 0;
		int readpoll = poll(servor.all_pollfd.data(), servor.all_pollfd.size(), 10);
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
			if (servor.all_pollfd[i].fd == servor.getFD())
				std::cout << "is a servor" << std::endl;
			if (servor.all_pollfd[i].revents & POLLERR)
				std::cout << "POLLERR" << std::endl;
			if (servor.all_pollfd[i].revents & POLLHUP)
				std::cout << "POLLHUP" << std::endl;
			if (servor.all_pollfd[i].revents & POLLNVAL)
				std::cout << "POLLNVAL" << std::endl;

			close(servor.all_pollfd[i].fd);
		}
	}
}

void	routine_servor(std::vector<serveur*> &servor, char **env)
{
	try
	{
		std::vector<serveur*>::iterator itserv = servor.begin();

		std::cout << BLUE << servor.size() << RESET << std::endl;
		while (true)
		{

			if (itserv == servor.end())
				itserv = servor.begin();
			else
				monitoring(*(*itserv++), env);
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
