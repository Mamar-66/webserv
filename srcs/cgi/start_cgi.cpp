/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmichel- <rmichel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:58:02 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/13 09:44:29 by rmichel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/Webserv.h"


#include <sys/wait.h>

void	config_cgi(monitoring &moni, client &cl)
{
	if (pipe(cl.pipe_write) == -1 || pipe(cl.pipe_read) == -1)
		throw std::runtime_error("Erreur de pipe");

	// std::cerr << BLUE "client fd = " << cl.getFD() << " " << "pipe fd == " << cl.pipe_write[0] << " " << cl.pipe_write[1] << RESET << std::endl; /* Uncomment if you want to see which client created a pipe (can create conditional jump if you uncomment !!! Not our fault, no guardrail there) */ 
	// std::cerr << BLUE "client fd = " << cl.getFD() << " " << "pipe fd == " << cl.pipe_read[0] << " " << cl.pipe_read[1] << RESET << std::endl;   /* Uncomment if you want to see which client created a pipe (can create conditional jump if you uncomment !!! Not our fault, no guardrail there) */

	cl.cgi_pollfd_write[0].fd = cl.pipe_write[0];
	cl.cgi_pollfd_write[0].events = POLLIN | POLLOUT;
	cl.cgi_pollfd_write[0].revents = 0;

	cl.cgi_pollfd_write[1].fd = cl.pipe_write[1];
	cl.cgi_pollfd_write[1].events = POLLIN | POLLOUT ;
	cl.cgi_pollfd_write[1].revents = 0;

	cl.cgi_pollfd_read[0].fd = cl.pipe_read[0];
	cl.cgi_pollfd_read[0].events = POLLIN | POLLOUT;
	cl.cgi_pollfd_read[0].revents = 0;

	cl.cgi_pollfd_read[1].fd = cl.pipe_read[1];
	cl.cgi_pollfd_read[1].events = POLLIN | POLLOUT ;
	cl.cgi_pollfd_read[1].revents = 0;

	if (fcntl(cl.cgi_pollfd_write[1].fd, F_SETFL, O_NONBLOCK) == -1)
		std::runtime_error(RED "fcntl F_SETFL");
	if (fcntl(cl.cgi_pollfd_read[0].fd, F_SETFL, O_NONBLOCK) == -1)
		std::runtime_error(RED "fcntl F_SETFL");

	moni.all_all_pollfd.push_back(cl.cgi_pollfd_write[1]);
	moni.all_all_pollfd.push_back(cl.cgi_pollfd_read[0]);

	pid_t pid = fork();
	if (pid == -1)
		throw std::runtime_error("Error pid -1");
	if (pid == 0)
	{
		if (dup2(cl.pipe_write[0], STDIN_FILENO) == -1)
			throw std::runtime_error(RED "error from dup2 stdin");

		close(cl.pipe_write[0]);
		close(cl.pipe_write[1]);

		if (dup2(cl.pipe_read[1], STDOUT_FILENO) == -1)
			throw std::runtime_error(RED "error from dup2 stdout");

		close(cl.pipe_read[0]);
		close(cl.pipe_read[1]);

		/* chemin du stript depuit l'uri */
		std::string envp_string =cl.envp[0];
		std::string path =  envp_string.substr(envp_string.find('=') + 1);

		if (execve(path.c_str(), cl.envp, cl.envp) == -1)
			throw std::runtime_error(BLUE "exerve failled");
	}
	else
	{
		close(cl.pipe_read[1]);
		cl.pid_child = pid;
	}
}

bool	start_CGI(monitoring &moni, client &cl)
{
	try
	{
		config_cgi(moni, cl);
		cl.setCgiTrue();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	return true;
}
