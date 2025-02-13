/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:29:52 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/12 17:25:57 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.hpp"
#include "../includes/config.hpp"


	std::time_t start = std::time(NULL);
	std::time_t newnext = 0;
void	init(int argc, char **argv, char **env)
{
	(void)env;
	monitoring moni;

	std::vector<pollfd>	allPollFd;

	std::vector<std::string> cut_str_serv = cut_conf_serv(argc, argv[1]);
	std::vector<serveur*> servors = creat_servor(cut_str_serv);

	if (!servors.empty())
	{
		for (int i = 0; i < (int)servors.size(); ++i)
		{
			for (int j = 0; j < (int)servors[i]->all_pollfd.size(); ++j)
			{
				allPollFd.push_back(servors[i]->all_pollfd[j]);
			}
		}
	}
	moni.all_pollfd_servor = allPollFd;
	moni.all_all_pollfd = allPollFd;

	routine_servor(moni, env);
}

int main(int argc, char **argv, char **env)
{
	try 
	{
		(void)env;
		init(argc, argv, env);

		// if (!servors.empty())
		// {
		// 	for(int i = 0; i < (int)servors.size(); ++i)
		// 		delete servors[i];
		// 	servors.clear();
		// }
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << e.what() <<  RESET << std::endl;
		return 1;
	}

	return 1;
}

