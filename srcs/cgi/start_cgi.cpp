/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:58:02 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/17 16:20:54 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/client.hpp"

#include <sys/wait.h>

// void	dublicat_envp(client &new_clien, client &cl)
// {
// 	int i = 0;

// 	std::cout << "deb : " << new_clien.envp[i] << std::endl;
// 	for (; cl.envp[i] != NULL; ++i)
// 	{
// 		std::cout << "deb : " << new_clien.envp[i] << std::endl;
// 		new_clien.envp[i] = strdup(cl.envp[i]);
// 	}
// 	// new_clien.envp[i] = NULL;
// }

void	config_cgi(monitoring &moni, client &cl)
{
	cl.setCgiTrue();

	if (pipe(cl.pipe_write) == -1 || pipe(cl.pipe_read) == -1)
	{
		perror("Erreur de pipe");
		return;
	}
	std::cout << BLUE "client fd = " << cl.getFD() << " " << "pipe fd == " << cl.pipe_write[0] << " " << cl.pipe_write[1] << std::endl;
	std::cout << BLUE "client fd = " << cl.getFD() << " " << "pipe fd == " << cl.pipe_read[0] << " " << cl.pipe_read[1] << std::endl;

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

	// moni.all_all_pollfd.push_back(cl.cgi_pollfd_write[0]);
	moni.all_all_pollfd.push_back(cl.cgi_pollfd_write[1]);
	moni.all_all_pollfd.push_back(cl.cgi_pollfd_read[0]);
	// moni.all_all_pollfd.push_back(cl.cgi_pollfd_read[1]);

	pid_t pid = fork();
	if (pid == -1)
		throw std::runtime_error("Error pid -1");
	if (pid == 0)
	{

		if (dup2(cl.pipe_write[0], STDIN_FILENO) == -1)
			throw std::runtime_error(RED "error from dup2 stdin");
		// close(cl.pipe_write[0]);
		
		if (dup2(cl.pipe_read[1], STDOUT_FILENO) == -1)
			throw std::runtime_error(RED "error from dup2 stdout");

		// if (dup2(cl.getFD(), STDOUT_FILENO) == -1)
		// 	throw std::runtime_error(RED "error from dup2 stdout");
		// if (dup2(cl.getFD(), STDIN_FILENO) == -1)
		// 	throw std::runtime_error(RED "error from dup2 stdint");

		/* chemin du stript depuit l'uri */
		std::string envp_string = cl.envp[0];
		size_t equal_pos = envp_string.find('=');
		std::string path = "." + envp_string.substr(equal_pos + 1);
		char *argv[2];
		argv[0] = strdup(path.c_str());
		argv[1] = NULL;

		if (execve(argv[0], argv, cl.envp) == -1)
		{
			perror("execve failed");
			std::cout << BLUE << argv[0] << RESET << std::endl;
		}

		// close(cl.pipe_read[1]);
		// close(cl.pipe_write[0]);
		// close(cl.pipe_read[1]);
	}
	else
	{
		
		// int status;
		// waitpid(pid, &status, 0);
		// write(new_client->getFD(), cl.getInput().c_str(), cl.getInput().length());
	}
}

void	start_CGI(monitoring &moni, client &cl)
{
	try
	{
		
		std::cout << RED "ouais ouais ouais" RESET << std::endl;
		config_cgi(moni, cl);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}
