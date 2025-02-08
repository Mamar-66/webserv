/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:29:52 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/08 12:04:10 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.hpp"
#include "../includes/config.hpp"

int main(int argc, char **argv, char **env)
{
	try 
	{
		(void)env;
		std::vector<std::string> cut_str_serv = cut_conf_serv(argc, argv[1]);
		std::vector<serveur*> servors = creat_servor(cut_str_serv);
		if (!servors.empty())
		{
			routine_servor(servors, env);
			for(int i = 0; i < (int)servors.size(); ++i)
				delete servors[i];
			servors.clear();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << e.what() <<  RESET << std::endl;
		return 1;
	}

	return 1;
}

