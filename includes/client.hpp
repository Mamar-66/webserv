/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:07:36 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/17 13:55:02 by omfelk           ###   ########.fr       */
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

	class	serveur;
	class	monitoring;
	class	cgi;


	class client
	{
		private:
			int				socket_fd;
			int				socket_fd_father;
			std::time_t 	startTime;
			std::string 	input;
			std::string 	output;
			size_t			size_body;
			bool			listing;
			bool			is_cgi;

		public:
			client(int fdsocket, size_t size_body, bool listing);
			client(int fdsocket, int fdSocketFather,size_t size_body, bool listing);
			~client();

						/* GETTER */
			const int			&getFD(void);
			const int 			&getFDFather();
			const std::time_t	&getStartTime();
			const std::string	&getInput(void);
			const std::string	&getOutput(void);
			const bool			&getStatusCgi();
			const size_t		&getSizeBody();
			const bool			&getListing();

			/* SETTER */
			void				setInput(const std::string& str);
			void				setOutput(const std::string& str);
			void				setCgiTrue();
			void				setCgiFalse();

			pollfd				clien_pollfd;
			pollfd				cgi_pollfd_write[2];
			pollfd				cgi_pollfd_read[2];
			bool 				write;

			char 				*envp[4];
			int					pipe_write[2];
			int					pipe_read[2];
			// cgi					*cg_i;
	};

	// class	cgi
	// {
	// 	private :
	// 		bool		im_cgi;
	// 		int			socket_fd;
	// 		void 		init_cgi();

	// 	public :
	// 		cgi();
	// 		~cgi();

	// 		const int &		getFfCgi();	

	// 		std::vector<std::string>	envp;
	// 		pollfd						pollfd_cgi;

	// };

	void	creat_client(monitoring &moni, int &fd, char **env);
	void	responding(monitoring &moni, int &fd, char **env, int i);
	void	error(monitoring &servor, pollfd &poll, int i);
	void	read_client(monitoring &moni, int&fd, int i);
	void	raph(monitoring &moni, client &cl, char** env);
	void 	start_CGI(monitoring &moni, client &cl);

#endif
