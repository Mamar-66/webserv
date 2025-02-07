/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:29:52 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/07 17:25:38 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.hpp"
#include "../includes/config.hpp"

int main(int argc, char **argv, char **env)
{
	try 
	{
		std::vector<std::string> cut_str_serv = cut_conf_serv(argc, argv[1]);
		std::vector<serveur> servors = creat_servor(cut_str_serv);
		routine_servor(servors, env);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() <<  RESET << std::endl;
		return 1;
	}

	return 1;
}

