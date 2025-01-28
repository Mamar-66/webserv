/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:06:45 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/28 13:34:33 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
#define SERVEUR_HPP

	#include "client.hpp"
	#include "config.hpp"

	#include <netinet/in.h>
	#include <unistd.h>
	#include <poll.h>

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <map>
	#include <errno.h>
	#include <sstream>

	#define MAX_CLIENTS 10

			/* COLOR */
	#define RED "\033[31m"
	#define ORANGE "\033[38;5;208m"
	#define GREEN "\033[32m"
	#define BLUE "\033[34m"
	#define RESET "\033[0m"
	#define BOLD "\033[1m"
	#define UNDERLINE "\033[4m"

	class client;

	class serveur : public config
	{
		private:
			sockaddr_in server_addr;

			int socket_fd;

			void 	addConfig(const std::string &strConfig);
			void	creatSocket();
			void	bindSocket();
			void	stratListening();

		public:
			//serveur();
			serveur(const std::string &strConfig);
			virtual ~serveur();

			std::map<int, client> clients;

			pollfd *pfd;
			// std::list<pollfd> pfd;

			int 			getFD();
			std::string		return_word_after(const std::string &word, const std::string &str);
			int				stringToInt(const std::string &str);
	};

	void					routine_servor(std::vector<serveur> &servor);
	std::vector<serveur>	creat_servor(std::vector<std::string> &cut_str_serv);

#endif
