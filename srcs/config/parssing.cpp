/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:04:12 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/16 10:50:00 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Webserv.h"

void removeComments(std::string& content)
{
    size_t commentPos;
    size_t lineStart = 0;
    std::string result = "";

    while ((commentPos = content.find('#', lineStart)) != std::string::npos)
    {
        size_t newlinePos = content.find('\n', commentPos);

        // Si un # est trouvé, on le coupe jusqu'à la fin de la ligne
        if (newlinePos != std::string::npos)
        {
            result += content.substr(lineStart, commentPos - lineStart);  // Partie avant le #
            result += "\n";  // Ajouter le saut de ligne
            lineStart = newlinePos + 1;  // Continuer après le saut de ligne
        }
        else
        {
            result += content.substr(lineStart, commentPos - lineStart);  // Partie avant le #
            break;  // Fin du contenu
        }
    }

    // Ajouter le reste du contenu après le dernier # (si présent)
    if (lineStart < content.length())
        result += content.substr(lineStart);

    content = result;  // Mettre à jour le contenu avec la version modifiée
}

std::string normalizeSpaces(const std::string &input)
{
    std::string result;
    bool inSpace = false;

    for (size_t i = 0; i < input.size(); ++i) {
		char c = input[i];
        if (c == ' ' || c == '\t') {
            if (!inSpace) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
        }
    }
    size_t start = result.find_first_not_of(" \t");
    size_t end = result.find_last_not_of(" \t");
    if (start == std::string::npos)
        return "";
    return result.substr(start, end - start + 1);
}

std::string removeEmptyLines(const std::string &input) {
    std::string result;
    std::istringstream stream(input);
    std::string line;

    while (std::getline(stream, line)) {
        if (!line.empty() && line.find_first_not_of(" \t") != std::string::npos) {
            result += line + "\n";
        }
    }
    return result;
}

std::vector<std::string> splitLines(const std::string &input) {
    std::vector<std::string> lines;
    size_t start = 0;
    size_t end = 0;

    while ((end = input.find('\n', start)) != std::string::npos) {
        lines.push_back(input.substr(start, end - start));
        start = end + 1;
    }

    if (start < input.length()) {
        lines.push_back(input.substr(start));
    }

    return lines;
}

void	config::parsconfig(std::string& fileContent, std::map<std::string, Location>& location, std::string& current)
{
	if (fileContent.compare(0, 12, "server_name ") == 0)
		this->initconfigName(fileContent);
	else if (fileContent.compare(0, 7, "listen ") == 0)
		this->initPort(fileContent);
	else if (fileContent.compare(0, 7, "server ") == 0)
		this->initconfig(fileContent);
	else if (fileContent.compare(0, 5, "host ") == 0)
		this->initHost(fileContent);
	else if (fileContent.compare(0, 5, "root ") == 0)
		this->initRoot(fileContent);
	else if (fileContent.compare(0, 6, "index ") == 0)
		this->initIndex(fileContent);
	else if (fileContent.compare(0, 11, "error_page ") == 0)
		this->initError_page(fileContent);
	else if (fileContent.compare(0, 7, "return ") == 0)
		this->initReturn(fileContent);
	else if (fileContent.compare(0, 10, "autoindex ") == 0)
		this->initAutoindex(fileContent);
	else if (fileContent.compare(0, 21, "client_max_body_size ") == 0)
		this->initClient(fileContent);
	else if (fileContent.compare(0, 9, "location ") == 0)
	{
		if (countWords(fileContent) != 3)
			throw std::runtime_error("Error : maybe too Much or not Enough, Location");
		std::vector<std::string> parts = splitt(fileContent, ' ');
		if (parts[1][0] != '/')
			throw std::runtime_error("Error : The second argument is invalid, it must begin with '/', Location");
		else if (parts[2][0] != '{' || parts[2][1])
			throw std::runtime_error("Error : It's not a '{', Location");
		else
			current = parts[1];
		location[current].initLocation(fileContent);
	}
	else if (fileContent.compare(0, 3, "{") == 0)
		this->initPart(fileContent);
	else if (fileContent.compare(0, 3, "}") == 0)
		this->initContrpart(fileContent);
	else if (fileContent.compare(0, 1, "#") == 0)
		return ;
	else
	{
		throw std::runtime_error("Error : Invalid Argument in the block server : " + fileContent );
	}
}

void	parsconfigL(std::string& fileContent, std::map<std::string, Location>& location, std::string& current)
{
	
	if (fileContent.compare(0, 14, "allow_methods ") == 0)
		location[current].initAllow_methods(fileContent);
	else if (fileContent.compare(0, 10, "autoindex ") == 0)
		location[current].initAutoindex(fileContent);
	else if (fileContent.compare(0, 21, "client_max_body_size ") == 0)
		location[current].initClient(fileContent);
	else if (fileContent.compare(0, 5, "root ") == 0)
		location[current].initRoot(fileContent);
	else if (fileContent.compare(0, 7, "return ") == 0)
		location[current].initReturn(fileContent);
	else if (fileContent.compare(0, 6, "index ") == 0)
		location[current].initIndex(fileContent);
	else if (fileContent.compare(0, 8, "cgi_ext ") == 0)
		location[current].initCgi_ext(fileContent);
	else if (fileContent.compare(0, 11, "error_page ") == 0)
		location[current].initError_page(fileContent);
	else if (fileContent.compare(0, 9, "location ") == 0)
	{
			if (countWords(fileContent) != 3)
			throw std::runtime_error("Error : maybe too Much or not Enough, Location");
		std::vector<std::string> parts = splitt(fileContent, ' ');
		if (parts[1][0] != '/')
			throw std::runtime_error("Error : The second argument is invalid, it must begin with '/', Location");
		else if (parts[2][0] != '{' || parts[2][1])
			throw std::runtime_error("Error : It's not a '{', Location");
		else
			current = parts[1];
		location[current].initLocation(fileContent);
	}
	else if (fileContent.compare(0, 3, "{") == 0)
		location[current].initPart(fileContent);
	else if (fileContent.compare(0, 3, "}") == 0)
		location[current].initContrpart(fileContent);
	else if (fileContent.compare(0, 1, "#") == 0)
		return ;
	else
	{
		throw std::runtime_error("Error : Invalid Argument in the block location : " + fileContent );
	}
}

std::vector<std::string> splitconfigs(const std::string &fileContent)
{
    if (fileContent.compare(0, 8, "server {") != 0)
        throw std::runtime_error("Error : No block of configuration 'server' found at start.");  
    std::vector<std::string> configs;
    size_t pos = 0;
	size_t extractedSize = 0;

    while ((pos = fileContent.find("server {", pos)) != std::string::npos) {
        size_t configStart = pos;
        size_t configEnd = std::string::npos;
        int bracketCount = 1;

        for (size_t i = configStart + 8; i < fileContent.size(); ++i) {
            if (fileContent[i] == '{')
			{
				if (fileContent[i - 2] == 'r')
				{
					configEnd = i - 7;
					break;
				}
                bracketCount++;
			}
            else if (fileContent[i] == '}') {
                bracketCount--;
                if (bracketCount == 0) {
                    configEnd = i + 1;
                    break;
                }
            }
        }
        if (configEnd == std::string::npos)
            configEnd = fileContent.size();

        std::string serverBlock = fileContent.substr(configStart, configEnd - configStart);
        configs.push_back(serverBlock);
		extractedSize += serverBlock.size() + 1;
         pos = configEnd;
    }
	if (extractedSize != fileContent.size())
        throw std::runtime_error("Error: Configuration contains lines outside of any 'server' block.");
    if (configs.empty())
        throw std::runtime_error("Error : No block of configuration 'server' found.");


    return configs;
}


void config::parseLocations(const std::vector<std::string>& lines, std::string& current)
{
    size_t locationCount = 0;
	std::string	chiant;

   for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        const std::string& line = *it;

        if (line.find("location") != std::string::npos) {
            locationCount++;
			chiant = normalizeSpaces(line);
            std::vector<std::string> parts = splitt(chiant, ' ');
            if (parts.size() >= 2) {
                std::string locationPath = parts[1]; 
				location.insert(std::make_pair(locationPath, Location()));
				if (locationCount == 1)
					current = parts[1];
            }
        }
    }
	if (locationCount != location.size() && current.empty() == 0)
			throw std::runtime_error("Error : same location");
}

std::vector<std::string> cut_conf_serv(const int argc, char *configFile)
{
	if (argc != 2)
		throw std::runtime_error("Error arg ex ./webserv [file config]");

	std::map<std::string, Location> location;
	config myconfig;
	std::string filename = configFile;
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		throw std::runtime_error("Error: unable to open file " + filename);
	}
	std::string fileContent;
	std::string line;
	while (std::getline(file, line))
		fileContent += line + "\n";
	file.close();
	try
	{
		removeComments(fileContent);
		fileContent = removeEmptyLines(fileContent);
		fileContent = normalizeSpaces(fileContent);
		std::vector<std::string> configs = splitconfigs(fileContent);
		return (configs);
	}
	catch(const std::exception& e)
	{
		throw;
	}
}
