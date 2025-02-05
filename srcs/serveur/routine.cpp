/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/05 10:47:21 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"


void	monitoring(serveur &servor, char **env)
{
	std::vector<pollfd> tmp_pollfd = servor.all_pollfd;
	int readpoll = poll(tmp_pollfd.data(), tmp_pollfd.size(), -1);
	if (readpoll < 0)
		throw std::runtime_error(RED "Error poll = -1");

	std::cout << ORANGE "server listen . . ." RESET << std::endl;

	for (std::vector<pollfd>::iterator itcl = tmp_pollfd.begin(); itcl != tmp_pollfd.end(); ++itcl)
	{
		if (itcl->revents & POLLIN)
		{
			if(itcl->fd == servor.getFD())
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
		}
		if(itcl->revents & POLLOUT)
		{
			try
			{
				responding(servor, itcl->fd);
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
