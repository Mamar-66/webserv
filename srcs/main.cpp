/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:29:52 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/22 15:44:06 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.hpp"
#include "../includes/config.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	try 
	{
		std::vector<std::string> cut_str_serv = cut_conf_serv(argc, argv[1]);
		std::vector<serveur> servors = creat_servor(cut_str_serv);
		// routine_servor();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() <<  RESET << std::endl;
		return 1;
	}

	return 1;
}
