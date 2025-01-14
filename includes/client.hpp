/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:07:36 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/14 20:17:29 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
	#define CLIENT_HPP

	#include "utiles.hpp"

	#include <sys/socket.h>
	#include <unistd.h>
	#include <poll.h>
	#include <fcntl.h>

	#include <iostream>
	#include <sstream>
	#include <cstring>


			/* COLOR */
	#define RED "\033[31m"
	#define ORANGE "\033[38;5;208m"
	#define GREEN "\033[32m"
	#define BLUE "\033[34m"
	#define RESET "\033[0m"
	#define BOLD "\033[1m"
	#define UNDERLINE "\033[4m"

class client
{
	private:
		int socket_fd;

	public:
		client(int fdsocket);
		~client();

		std::string input;
		pollfd pfd;

		// std::list<pollfd> pfd;
	};

	client *creat_client(int fd_serveur);

#endif
