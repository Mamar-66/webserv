/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:58:02 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/15 19:43:29 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/client.hpp"

void	config_cgi(monitoring &moni, client &cl)
{
	(void)moni;
	cl.setCgiTrue();

	cl.cg_i->envp.push_back(cl.getInput());
}

void	start_CGI(monitoring &moni, client &cl)
{
	try
	{
		cl.cg_i = new cgi();
		config_cgi(moni, cl);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
}
