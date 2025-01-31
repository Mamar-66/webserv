/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:07:36 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/31 17:11:46 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
	#define CLIENT_HPP

	//#include "utiles.hpp"
	#include "serveur.hpp"

	#include <sys/socket.h>
	#include <unistd.h>
	#include <poll.h>
	#include <fcntl.h>

	#include <iostream>
	#include <sstream>
	#include <fstream>
	#include <cstring>
	#include <cstdlib>
	#include <ctime>


			/* COLOR */
	#define RED "\033[31m"
	#define ORANGE "\033[38;5;208m"
	#define GREEN "\033[32m"
	#define BLUE "\033[34m"
	#define RESET "\033[0m"
	#define BOLD "\033[1m"
	#define UNDERLINE "\033[4m"

	class serveur;

	class client
	{
		private:
			int				socket_fd;
			std::time_t 	startTime;

		public:
			client();
			client(int fdsocket);
			~client();

			std::string 	input;
			std::string 	output;
			pollfd			pfd;

			int				getFD();
	};

	void		creat_client(int fd_serveur);
	//void		client_existed(int &fd_client, serveur &servor);

#endif
