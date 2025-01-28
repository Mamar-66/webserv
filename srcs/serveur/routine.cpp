/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/28 15:46:29 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"

void	routine_servor(std::vector<serveur> &servor)
{
	try
	{
		std::vector<serveur>::iterator itserv = servor.begin();

		while (true)
		{
			int readpoll = poll(itserv->pfd, MAX_CLIENTS + 1, -1);
			if (readpoll < 0)
				throw std::runtime_error(RED "Error poll = -1");

			for (int i = 0; i <= MAX_CLIENTS; i++)
			{
				
				std::cout << "i = " << i << std::endl;
				if (itserv->pfd[i].revents & POLLIN)
				{
					if (itserv->pfd[i].fd == itserv->getFD())
					{
						creat_client(itserv->getFD(), *itserv);
					}
					else
					{
						client_existed(itserv->pfd[i].fd, *itserv);
					}
				}
			}
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
