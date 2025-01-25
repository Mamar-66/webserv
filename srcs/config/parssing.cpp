/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:04:12 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/25 18:50:18 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config.hpp"
#include <unistd.h>

std::string cut_serv(std::ifstream &file)
{
	std::string return_str;
	std::string line;
	bool		in_block = false;
	int			nbKey = 0;
	int			ligne_cumpte = 0;
	std::ostringstream oss;

	while (std::getline(file, line))
	{
		ligne_cumpte++;

		if (line.empty() || line[0] == '#') continue;
		line.erase(line.find_last_not_of(" \t") + 1);

		if (line == "server {" || line == "server	{" || in_block)
		{
			return_str += line;
			return_str += "\n";

			if ((line == "server {" || line == "server	{") && !in_block)
				in_block = true;

			if (line.find('{') != std::string::npos)
				nbKey++;
			else if (line.find('}') != std::string::npos)
				nbKey--;

			if (nbKey > 2 || nbKey < 0)
			{
				oss << ligne_cumpte;
				throw std::runtime_error(RED "Error nb { or } ligne : " + oss.str());
			}

			if (nbKey == 0)
				break;

			if (line[line.length() - 1] != ';' && (line[line.length() - 1] != '{' && line[line.length() - 1] != '}'))
			{
				oss << ligne_cumpte;
				throw std::runtime_error(RED "Error forget ; ligne : " + oss.str());
			}
		}
	}
	return return_str;
}

/* return vector text serveur { } */

std::vector<std::string> cut_conf_serv(const int argc, char *configFile)
{
	std::vector<std::string> return_parss;

	if (argc != 2)
		throw std::runtime_error(RED "Error arg ex " ORANGE "./webserv [file config]" RESET);

	std::ifstream	file(configFile);
	if (!file.is_open())
		throw std::runtime_error(RED "Error opening file .conf" RESET);

	while (!file.eof())
	{
		try
		{
			write(1, "ok\n", 3);
			return_parss.push_back(cut_serv(file));
		}
		catch(const std::exception& e)
		{
			file.close();
			throw;
		}
	}

	file.close();
	return return_parss;
}
