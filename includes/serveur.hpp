/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmichel- <rmichel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:06:45 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/13 09:31:32 by rmichel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
# define SERVEUR_HPP

	#include <netinet/in.h>
	#include <unistd.h>
	#include <poll.h>

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <map>
	#include <vector>
	#include <set>
	#include <errno.h>
	#include <sstream>
	#include <csignal>

	#include <ctime>
	
	// #include "config.hpp"
	#include "Webserv.h"

	extern bool 		running;
	extern std::time_t	start;
	extern std::time_t	newnext;

	#define MAX_CLIENTS 10000
	#define TIMEOUT 10

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
			std::vector<int>	servor_socket;

			void 			addConfig(const std::string &strConfig);
			int				creatSocket(const int &port);
			int				listen_port(const int &port);
			void			bind_port();

			serveur(serveur& cpy);
			serveur& operator=(serveur& cpy);

		public:
			serveur(const std::string &strConfig);
			virtual ~serveur();

					/* GETTER */
			pollfd					pfd;
			std::vector<pollfd>		all_pollfd;
			std::string				return_word_after(const std::string &word, const std::string &str);
	};

	class monitoring
	{
		private :

		public :
			monitoring();
			~monitoring();

			std::vector<pollfd>		all_pollfd_servor;
			std::vector<pollfd>		all_all_pollfd;
			std::map<int, client*>	clients;
			std::map<int, serveur*>	servors;

			std::map<std::string, Cookie> mapCookie;

			int						where_are_fd_pipe(const int &fd);
	};

	void		routine_servor(monitoring &moni);

#endif
