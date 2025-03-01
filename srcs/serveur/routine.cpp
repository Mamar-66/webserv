/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/01 14:16:45 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"
#include "../../includes/Parser.hpp"

void	monitorin(monitoring &moni)
{
	size_t i = -1;

	// std::cout << ORANGE "serveur listen . . ." RESET << std::endl;
	while (++i < moni.all_all_pollfd.size() && running)
	{
		for (int i = 0; i < (int)moni.all_all_pollfd.size(); i++)
			moni.all_all_pollfd[i].revents = 0;

		int readpoll = poll(moni.all_all_pollfd.data(), moni.all_all_pollfd.size(), -1);
		if (readpoll < 0)
			throw std::runtime_error(RED "Error poll = -1");

		if (!Checker::compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor))
		{
			if (Checker::time_out(moni, moni.all_all_pollfd[i].fd, i))
			{
				if (i + 1 >= moni.all_all_pollfd.size())
					i = -1;
				continue;
			}
		}
		if (Checker::compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor) && moni.all_all_pollfd[i].revents & POLLIN)
		{
			creat_client(moni, moni.all_all_pollfd[i].fd);
		}
		else if (!Checker::compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor)
			&& (moni.all_all_pollfd[i].revents & POLLIN) != 0)
		{
			read_client(moni, moni.all_all_pollfd[i].fd, i);
		}
		else if (!Checker::compar(moni.all_all_pollfd[i].fd, moni.all_pollfd_servor)
			&& (moni.all_all_pollfd[i].revents & POLLOUT) != 0)
		{
			responding(moni, moni.all_all_pollfd[i].fd, i);
		}
		else if (moni.all_all_pollfd[i].revents & POLLERR
			|| moni.all_all_pollfd[i].revents & POLLHUP
			|| moni.all_all_pollfd[i].revents & POLLNVAL
			|| moni.all_all_pollfd[i].revents & POLLRDHUP)
		{
			error(moni, moni.all_all_pollfd[i], i);
		}
		if (i + 1 >= moni.all_all_pollfd.size())
			i = -1;
	}
}

void routine_servor(monitoring &moni)
{
	try
	{
		monitorin(moni);
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
