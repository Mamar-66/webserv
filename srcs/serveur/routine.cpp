/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:04:20 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/30 15:24:53 by omfelk           ###   ########.fr       */
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
			int readpoll = poll(&itserv->pfd, 1, -1);
			if (readpoll < 0)
				throw std::runtime_error(RED "Error poll = -1");

			if (itserv->pfd.revents & POLLIN)
			{
				if (itserv->pfd.fd == itserv->getFD())
				{
					creat_client(itserv->getFD());
				}
			}
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
