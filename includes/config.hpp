/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:54:40 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/24 12:06:51 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	config_hpp
	#define	config_hpp

	#include <string>
	#include <iostream>
	#include <vector>
	#include <fstream>
	#include <sstream>

		/* COLOR */
	#define RED "\033[31m"
	#define ORANGE "\033[38;5;208m"
	#define GREEN "\033[32m"
	#define BLUE "\033[34m"
	#define RESET "\033[0m"
	#define BOLD "\033[1m"
	#define UNDERLINE "\033[4m"

	class config
	{
		protected :
			int 		port;
			std::string host;
			std::string host_name;
			// int 		client_max_body_size;

		public :
			config();
			~config();
	};

	std::vector<std::string> cut_conf_serv(const int argc, char *configFile);

#endif
