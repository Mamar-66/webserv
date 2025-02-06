/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:07:36 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/06 10:39:10 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
	#define CLIENT_HPP

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
			std::time_t 	startTime;
			std::string 	input;
			std::string 	output;
			int				socket_fd;
			bool			is_cgi;

		public:
			client(int fdsocket);
			~client();

						/* GETTER */
			const int			&getFD(void);
			const std::time_t	&getStartTime();
			const std::string	&getInput(void);
			const std::string	&getOutput(void);
			const bool			&getStatusCgi();

			/* SETTER */
			void				setInput(const std::string& str);
			void				setOutput(const std::string& str);
			void				setStatusCgiTrue();

			pollfd			clien_pollfd;
	};

	void	creat_client(serveur &servor, char **env);
	void	responding(serveur &servor, int &fd);

#endif
