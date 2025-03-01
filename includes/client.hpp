/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:07:36 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/01 15:52:17 by omfelk           ###   ########.fr       */
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
			bool			timeout;
			std::string		location;

		public:
			client(int fdsocket, int fdFather);
			~client();

			size_t		content_lenght;
			bool		chunked;
			bool		first_pass;
			bool		responce_cgi;
			bool		client_close;
			size_t		content_real;

						/* GETTER */
			const int				&getFD( void );
			const int 				&getFDFather( void );
			const std::time_t		&getStartTime( void );
			const std::string		&getInput( void );
			const std::string		&getOutput( void );
			const bool				&getStatusCgi( void );
			const bool				&getTimeoutBool( void );
			const std::string		&getGoodLocation( void );

			/* SETTER */
			void				setInput(const std::string& str);
			void				setOutput(const std::string& str);
			void				setCgiTrue();
			void				setCgiFalse();
			void				setActualTime();
			void				setTimeoutBool( bool state );
			void				setEnv(char **env);
			void				setGoodLocation( std::string Location);

			void				AddInput(const std::string &str);

			pollfd				clien_pollfd;
			pollfd				cgi_pollfd_write[2];
			pollfd				cgi_pollfd_read[2];
			bool 				write;

			char 				**envp;
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
	bool	start_CGI(monitoring &moni, client &cl);

#endif
