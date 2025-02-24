/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/24 12:58:30 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"

void	monitorin(monitoring &moni)
{
	size_t i = -1;

	// std::cout << ORANGE "serveur listen . . ." RESET << std::endl;
	while (++i < moni.all_all_pollfd.size())
	{
		int readpoll = poll(moni.all_all_pollfd.data(), moni.all_all_pollfd.size(), -1);
		if (readpoll < 0)
			throw std::runtime_error(RED "Error poll = -1");

		if (!compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor))
		{
			if (time_out(moni, moni.all_all_pollfd[i].fd, i))
				continue;
		}
		if (compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor) && moni.all_all_pollfd[i].revents & POLLIN)
		{
			creat_client(moni, moni.all_all_pollfd[i].fd);
		}
		else if (!compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor)
			&& (moni.all_all_pollfd[i].revents & POLLIN) != 0)
		{
			read_client(moni, moni.all_all_pollfd[i].fd, i);
		}
		else if (!compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor)
			&& (moni.all_all_pollfd[i].revents & POLLOUT) != 0)
		{
			responding(moni, moni.all_all_pollfd[i].fd, i);
		}
		else if (moni.all_all_pollfd[i].revents & POLLERR
			|| moni.all_all_pollfd[i].revents & POLLHUP
			|| moni.all_all_pollfd[i].revents & POLLNVAL)
		{
			error(moni, moni.all_all_pollfd[i], i);
		}
	}
}

void routine_servor(monitoring &moni)
{
	try
	{
		while (running)
		{
			for (int i = 0; i < (int)moni.all_all_pollfd.size(); i++)
				moni.all_all_pollfd[i].revents = 0;

			monitorin(moni);
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
