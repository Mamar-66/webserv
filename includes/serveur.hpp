/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:06:45 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/14 11:03:29 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_HPP
#define SERVEUR_HPP

	#include "client.hpp"
	#include "config.hpp"
	#include "Location.hpp"

	#include <netinet/in.h>
	#include <unistd.h>
	#include <poll.h>

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <map>
	#include <errno.h>
	#include <sstream>
	#include <csignal>

	#include <ctime>
	
	extern bool 		running;
	extern std::time_t start;
	extern std::time_t newnext;


	#define MAX_CLIENTS 1000

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

		public:
			serveur(const std::string &strConfig);
			virtual ~serveur();

					/* GETTER */
			pollfd					pfd;
			std::vector<pollfd>		all_pollfd;
			std::string				return_word_after(const std::string &word, const std::string &str);
			int						stringToInt(const std::string &str);
	};

	class monitoring
	{
		private :

		public :
			monitoring(std::vector<pollfd> allPollFd);
			~monitoring();

			char **cpy_env;

			std::vector<pollfd>		all_pollfd_servor;
			std::vector<pollfd>		all_all_pollfd;
			std::map<int, client*>	clients;
	};

	void					routine_servor(monitoring &moni, char **env);
	std::vector<serveur*>	creat_servor(std::vector<std::string> &cut_str_serv);
	bool 					compar(const int &fd, const std::vector<pollfd> &poll_servor);

#endif
