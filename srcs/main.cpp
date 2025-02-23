/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:29:52 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/23 11:06:44 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.hpp"
#include "../includes/config.hpp"

// docker run --rm -t jstarcher/siege -b -c100 -r200 10.13.5.2:8080 | grep -v "HTTP"
// valgrind --leak-check=full --track-fds=yes ./Webserv config/default.conf

std::time_t 	start = std::time(NULL);
std::time_t 	newnext = 0;
bool 			running = true;

void	signalHandler_ctrC(int signal)
{
	std::cout << RED "Close servor" RESET << std::endl;

	if (signal == SIGINT)
		running = false;
}

void init(int argc, char **argv)
{
	monitoring	moni;

	std::vector<std::string> cut_str_serv = cut_conf_serv(argc, argv[1]);
	creat_servor(cut_str_serv, moni);

	try 
	{
		if (moni.servors.empty())
			throw std::runtime_error(RED "Error servors not existed");

		routine_servor(moni);
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
}

int main(int argc, char **argv)
{
	try 
	{
		std::signal(SIGINT, signalHandler_ctrC);
		init(argc, argv);
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}

	return 1;
}
