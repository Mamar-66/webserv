/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:00:02 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/16 10:49:36 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config.hpp"

#include "config.hpp"
#include "Location.hpp"

config::config(void) : port(0), config_name("Default"), autoindex(false), verifauto(false), client_max_body_size(0)
{
	op = false;
}

config::config(const config &other)
{
	if (this != &other)
	{
		this->op = other.op;
		this->config_name = other.config_name;
		this->root = other.root;
		this->host = other.host;
		this->port = other.port;
		this->index = other.index;
		this->error_page = other.error_page;
		this->autoindex = other.autoindex;
		this->client_max_body_size = other.client_max_body_size;
	}
}

config& config::operator=(const config &other)
{
	if (this != &other)
	{
		this->op = other.op;
		this->config_name = other.config_name;
		this->root = other.root;
		this->host = other.host;
		this->port = other.port;
		this->index = other.index;
		this->error_page = other.error_page;
		this->autoindex = other.autoindex;
		this->client_max_body_size = other.client_max_body_size;
	}
	return (*this);
}

config::~config(void)
{
}

void	config::initconfig(std::string& fileContent)
{
	if (fileContent.compare(6, 1, "") != 0 && fileContent.compare(7, 3, "{") != 0)
		throw std::runtime_error("Error : Invalid, server");
	if (fileContent.compare(7, 3, "{") == 0)
		op = true;
}

void	config::initPort(std::string& fileContent)
{
	if (fileContent.compare(6, 1, "") == 0)
		throw std::runtime_error("Error : Missing Argument, listen");
	for (size_t i = 0; i < 4; i++)
	{
		if (fileContent[7 + i] > '9' || fileContent[7 + i] < '0')
			throw std::runtime_error("Error : Only [0;9], listen");
	}
	if (fileContent.compare(11, 3, ";") != 0)
		throw std::runtime_error("Error : Max 4 numbers or invalid endline ';', listen");
	std::string portStr = fileContent.substr(7, 4);
	int	i = std::atoi(portStr.c_str());
    this->port.push_back(i);
	if (i == 0 && portStr != "0")
        throw std::runtime_error("Error : conversion fail, listen");
}

void	config::initconfigName(std::string& fileContent)
{
	if (!config_name.empty() && config_name.compare(0, 7, "Default") != 0)
		 throw std::runtime_error("Error : is duplicated, server_name");
	if (countWords(fileContent) != 2)
		throw std::runtime_error("Error : Maybe too much Argument or not Enough, server_name");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline only ';' accepted, server_name");
	else
		config_name = fileContent.substr(12, fileContent.length() - 12 - 1);
	if (config_name.empty())
		throw std::runtime_error("Error : Missing the name, server_name");
}

void	config::initHost(std::string& fileContent)
{
	if (!host.empty())
		 throw std::runtime_error("Error : is duplicated, host");
	if (countWords(fileContent) != 2)
		throw std::runtime_error("Error : Too much Argument or not enough, host");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline only ';' accepted, host");
	else if (fileContent.length() - 6 > 15)
		throw std::runtime_error("Error : Argument is too long, xxx.xxx.xxx.xxx is the max, host");
	for (size_t i = 0; i < fileContent.length() - 6; i++)
	{
		if ((fileContent[5 + i] > '9' && fileContent[5 + i] != '.') || (fileContent[5 + i] < '0' && fileContent[5 + i] != '.'))
			throw std::runtime_error("Error : Invalid address, host");
		if ((fileContent[4 + i] == '.' && fileContent[5 + i] == '0' && fileContent[6 + i] >= '0') && (fileContent[5 + i] == '0' && fileContent[6 + i] <= '9' && fileContent[4 + i] == '.'))
			throw std::runtime_error("Error : Invalid address, host");
	}
	std::string adjustedContent = fileContent.substr(5);
	std::vector<std::string> parts = splitt(adjustedContent, '.');

    if (parts.size() != 4)
     throw std::runtime_error("Error : Address ip must have 4 parts, host");

    for (std::size_t i = 0; i < parts.size(); ++i) {
        int value = std::atoi(parts[i].c_str());
        if (value < 0 || value > 255) {
           throw std::runtime_error("Error : Invalid part: must be in the range [0;255], host");
        }
    }
	this->host = fileContent.substr(5, fileContent.length() - 5 - 1);
}

void	config::initRoot(std::string& fileContent)
{
	if (!root.empty())
		 throw std::runtime_error("Error : is duplicated, root");
	if (countWords(fileContent) != 2)
		throw std::runtime_error("Error : Too much Argument or not Enough, root");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline, only ';' accepted, root");
	else
		root = fileContent.substr(5, fileContent.length() - 5 - 1);
	if (root.empty())
		throw std::runtime_error("Error : Missing direction, root");
}

void	config::initIndex(std::string& fileContent)
{
	if (!index.empty())
		 throw std::runtime_error("Error : is duplicated, index");
	if (countWords(fileContent) != 2)
		throw std::runtime_error("Error : Too much Argument or not Enough, index");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline, only ';' accepted, index");
	else
		index = fileContent.substr(6, fileContent.length() - 6 - 1);
	if (index.empty())
		throw std::runtime_error("Error : Missing direction, index");
}

// std::vector<int> getErrorCodesForName(const std::map<std::vector<int>, std::string>& error_page, const std::string& name)
// {
//     for (std::map<std::vector<int>, std::string>::const_iterator it = error_page.begin(); it != error_page.end(); ++it) {
//         const std::vector<int>& key = it->first;
//         const std::string& value = it->second;
//         if (value == name)
//             return key;
//     }
//     throw std::runtime_error("Name not found in map");
// }

void config::initError_page(std::string& fileContent)
{
	if (countWords(fileContent) < 3)
		throw std::runtime_error("Error : Not Enough Argument, error_page");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline, only ';' accepted, error_page");
	std::string adjustedContent = fileContent.substr(11);
	std::vector<std::string> parts = splitt(adjustedContent, ' ');
	std::vector<int> error_codes;
	std::string	name;
	int	k = 0;
	for (size_t l = 0; l < parts.size() - 1; l++)
	{
		if (parts[l].length() != 3)
			throw std::runtime_error("Error : Second Argument invalid, too short or too long and must be in the range[0;9], error_page");
		for (size_t i = 0; i < 3; i++)
		{
			if (parts[l][i] > '9' || parts[l][i] < '0')
				throw std::runtime_error("Error : Second Argument must be in the range[0;9], error_page");
		}
   	 	int value = std::atoi(parts[l].c_str());
   	 	if (value < 400 || value > 599)
		{
   	         throw std::runtime_error("Error : Second Argument must be in the range [400;599], error_page");
   		}
		error_codes.push_back(value);
		k++;
	}
	name = fileContent.substr(11 + (3 * k) + (1 * k), fileContent.length() - (12 + (3 * k) + (1 * k)));
	error_page.insert(std::make_pair(error_codes, (name)));
	if (name.empty())
		throw std::runtime_error("Error : Missing direction, error_page");
	//std::vector<int> found_codes = getErrorCodesForName(error_page, "ClientErrors");
    //     // Affiche tous les codes d'erreur
    //     std::cout << "Found error codes: ";
    //     for (std::vector<int>::iterator it = found_codes.begin(); it != found_codes.end(); ++it) {
    // int code = *it;
    // // Traitement
    //         std::cout << code << " ";
    //     }
    //     std::cout << std::endl;
}

void	config::initAutoindex(std::string& fileContent)
{
	if (verifauto == true)
		 throw std::runtime_error("Error : is duplicated, autoindex");
	if (countWords(fileContent) != 2)
		throw std::runtime_error("Error : Too much Argument or not Enough, autoindex");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline, only ';' accepted, autoindex");
	else if (fileContent.compare(10, 5, "on;") != 0 && fileContent.compare(10, 5, "off;") != 0)
		throw std::runtime_error("Error : only 'on' or off' accepted, autoindex");
	else if (fileContent.compare(10, 5, "on;") == 0)
	{
		verifauto = true;
		autoindex = true;
	}
	else if (fileContent.compare(10, 5, "off;") == 0)
	{
		autoindex = false;
		verifauto = true;
	}
}

void	config::initClient(std::string& fileContent)
{
	if (client_max_body_size != 0)
		 throw std::runtime_error("Error : is duplicated, client_max_body_size");
	if (countWords(fileContent) != 2)
		throw std::runtime_error("Error : Too much Argument or not Enough, client_max_body_size");
	std::vector<std::string> parts = splitt(fileContent, ' ');
    if (parts[0] != "client_max_body_size")
        throw std::runtime_error("Error : Invalid, client_max_body_size");
    std::string sizeValue = parts[1];
    std::string numberPart = sizeValue.substr(0, sizeValue.size() - 1);
	for (size_t i = 0; i < numberPart.size(); ++i) {
        if (!std::isdigit(numberPart[i])) {
            throw std::runtime_error("Error : Number must be in the range [0;9], client_max_body_size");
        }
    }
    long int value = 0;
    for (size_t i = 0; i < numberPart.size(); ++i) {
        value = value * 10 + (numberPart[i] - '0');
        if (value > INT_MAX) {
			throw std::runtime_error("Error : greater than Int_MAX, client_max_body_size");
        }
    }
	int size = std::atoi(numberPart.c_str());
    if (size <= 0) {
         throw std::runtime_error("Error : greater than 0, client_max_body_size");
    }
	client_max_body_size = size;
}

void	config::initPart(std::string& fileContent)
{
	if (countWords(fileContent) != 1)
		throw std::runtime_error("Error : problem with, {");
	else if (fileContent[0] != '{' || fileContent[1])
		throw std::runtime_error("Error : problem with, {");
	else if (op == true)
		throw std::runtime_error("Error : Two '{', {");
	else
		op = true;
}

void	config::initContrpart(std::string& fileContent)
{
	if (countWords(fileContent) != 1)
		throw std::runtime_error("Error : problem with, }");
	else if (fileContent[0] != '}' || fileContent[1])
		throw std::runtime_error("Error : problem with, }");
	else if (op == false)
		throw std::runtime_error("Error : Two '}', }");
	else
		op = false;
}

bool config::getOp() const
{
    return op;
}

std::vector<int> config::getPort() const
{
    return port;
}

std::string config::getConfigName() const
{
    return config_name;
}

std::string config::getHost() const
{
    return host;
}

std::string config::getRoot() const
{
    return root;
}

std::string config::getIndex() const 
{
    return index;
}

std::map<std::vector<int>, std::string> config::getErrorPage() const
{
    return error_page;
}

bool config::getAutoindex() const
{
    return autoindex;
}

bool config::getVerifauto() const
{
    return verifauto;
}

int config::getClientMaxBodySize() const
{
    return client_max_body_size;
}
