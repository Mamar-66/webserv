/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:07:36 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/24 13:01:06 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
	#define CLIENT_HPP

	#include "serveur.hpp"
	#include "Utils.hpp"
	#include "Cookie.hpp"

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
			bool			is_cgi;


		public:
			client(int fdsocket, int fdFather);
			~client();

			size_t		content_lenght;
			bool		chunked;
			bool		first_pass;
			size_t		content_real;

						/* GETTER */
			const int				&getFD( void );
			const int 				&getFDFather( void );
			const std::time_t		&getStartTime( void );
			const std::string		&getInput( void );
			const std::string		&getOutput( void );
			const bool				&getStatusCgi( void );
			const size_t			&getSizeBody( void );
			const bool				&getListing( void );

			/* SETTER */
			void				setInput(const std::string& str);
			void				setOutput(const std::string& str);
			void				setCgiTrue();
			void				setCgiFalse();
			void				setActualTime();

			pollfd				clien_pollfd;
			pollfd				cgi_pollfd_write[2];
			pollfd				cgi_pollfd_read[2];
			bool 				write;

			char 				*envp[4];
			int					pipe_write[2];
			int					pipe_read[2];

			std::string 		read_request(const int &fd_client);
			std::string			read_request_cgi(const int &fd_client);
	};

	void	creat_client(monitoring &moni, int &fd);
	void	responding(monitoring &moni, int &fd, int i);
	void	error(monitoring &servor, pollfd &poll, int i);
	void	read_client(monitoring &moni, int &fd, int i);
	bool	raph(monitoring &moni, client &cl);
	void 	start_CGI(monitoring &moni, client &cl);
	bool	time_out(monitoring &moni, int fd, int i);

#endif
