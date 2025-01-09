/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:06:45 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/09 21:15:42 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
	#define SERVEUR_HPP

	#include "client.hpp"

	#include <sys/socket.h>
	#include <unistd.h>

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <list>
	#include <errno.h>
	#include <sstream> 

	#define PORT 8080

			/* COLOR */
	#define RED     "\033[31m"
	#define ORANGE  "\033[38;5;208m"
	#define GREEN   "\033[32m"
	#define BLUE    "\033[34m"
	#define RESET   "\033[0m"
	#define BOLD    "\033[1m"
	#define UNDERLINE "\033[4m"



	class serveur
	{
		private :
			std::list<client> clients;
			int socket_fd;

		public :
			serveur();
			~serveur();
		
	};


	void	routine_servor();

#endif
