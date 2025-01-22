/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_servors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:10:10 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/22 15:53:04 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/serveur.hpp"

std::vector<serveur> creat_servor(std::vector<std::string> &cut_str_serv)
{
	std::vector<serveur> servors;

	// std::vector<std::string>::iterator it;

	// for (it = cut_str_serv.begin(); it != cut_str_serv.end(); ++it)
	// 	std::cout << *it << std::endl;

	std::vector<std::string>::iterator itt;

	itt = cut_str_serv.begin();

	serveur server(*itt);
	

	return servors;
}
