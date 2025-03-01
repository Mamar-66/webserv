/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:19:17 by sbarbe            #+#    #+#             */
/*   Updated: 2025/02/21 22:34:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "config.hpp"

Location::Location() :	 autoindex(false), verifauto(false), client_max_body_size(-1)
{
	op = false;
}

Location::~Location() {
}

Location::Location(const Location &other)
{
	if (this != &other)
	{
		this->op = other.op;
		this->root = other.root;
		this->index = other.index;
		this->autoindex = other.autoindex;
		this->client_max_body_size = other.client_max_body_size;
		this->allow_methods = other.allow_methods;
		this->cgi_ext = other.cgi_ext;
		this->cgi_path = other.cgi_path;
		this->retur = other.retur;
		this->passwordDependent = other.passwordDependent;
	}
}

Location& Location::operator=(const Location &other)
{
	if (this != &other)
	{
		this->op = other.op;
		this->root = other.root;
		this->index = other.index;
		this->autoindex = other.autoindex;
		this->client_max_body_size = other.client_max_body_size;
		this->allow_methods = other.allow_methods;
		this->cgi_ext = other.cgi_ext;
		this->cgi_path = other.cgi_path;
		this->retur = other.retur;
		this->passwordDependent = other.passwordDependent;
	}
	return (*this);
}

void	Location::initIndex(std::string& fileContent)
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


void	Location::initRoot(std::string& fileContent)
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

void	Location::initAllow_methods(std::string& fileContent)
{
	if (!allow_methods.empty())
		 throw std::runtime_error("Error : is duplicated, allow_methods");
	if (countWords(fileContent) == 1)
		throw std::runtime_error("Error : not Enough Argument, allow_methods");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline, only ';' accepted, allow_methods");
	std::string adjustedContent = fileContent.substr(14);
	std::vector<std::string> parts = splitt(adjustedContent, ' ');
	for (size_t i = 0; i < parts.size(); i++)
	{
		if (parts[i].compare("GET") != 0 && parts[i].compare("DELETE") != 0 && parts[i].compare("POST") != 0 && parts[i].compare("PUT") != 0 && parts[i].compare("HEAD") != 0 && (i - 1 == parts.size() && parts[i].compare("GET;") != 0 && parts[i].compare("DELETE;") != 0 && parts[i].compare("POST;") != 0 && parts[i].compare("PUT;") != 0 && parts[i].compare("HEAD;") != 0))
			throw std::runtime_error("Error : only GET, DELETE, POST, PUT, HEAD, allow_methods");
		else
		{
			if (parts[i].compare(parts[i].length() - 1, 3, ";") == 0)
				allow_methods.push_back(parts[i].substr(0, parts[i].length() - 1));
			else
				allow_methods.push_back(parts[i].substr());
		}
	}
}

void	Location::initClient(std::string& fileContent)
{
	if (client_max_body_size != -1)
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

void	Location::initReturn(std::string& fileContent)
{
	if (!retur.empty())
		 throw std::runtime_error("Error : is duplicated, return");
	if (countWords(fileContent) != 2)
		throw std::runtime_error("Error : Too much Argment or not enough, return");
	else if (fileContent.compare(fileContent.length() - 1, 3, ";") != 0)
		throw std::runtime_error("Error : Invalid endline, only ';' accepted, return");
	retur = fileContent.substr(7, fileContent.length() - 7 - 1);
	if (retur.empty())
		throw std::runtime_error("Error : Missing direction, return");
}

void	Location::initCgi_path(std::string& fileContent)
{
	std::vector<std::string> parts = splitt(fileContent, ' ');
	for (size_t i = 1; i < parts.size(); ++i)
	{
		if (i + 1 == parts.size())
			cgi_path.push_back(parts[i].substr(0, parts[i].length() - 1));
		else
   			cgi_path.push_back(parts[i]);
	}
	if (cgi_path[0].empty())
		throw std::runtime_error("Error : Missing varible, cgi_path");
}

void	Location::initCgi_ext(std::string& fileContent)
{
	std::vector<std::string> parts = splitt(fileContent, ' ');
	for (size_t i = 1; i < parts.size(); ++i)
	{
		if (i + 1 == parts.size())
			cgi_ext.push_back(parts[i].substr(0, parts[i].length() - 1));
		else
   			cgi_ext.push_back(parts[i]);
	}
	if (cgi_ext[0].empty())
		throw std::runtime_error("Error : Missing varible, cgi_ext");
}

void	Location::initLocation(std::string& fileContent)
{
	if (countWords(fileContent) != 3)
		throw std::runtime_error("Error : Too much argument or not Enough, location");
	std::vector<std::string> parts = splitt(fileContent, ' ');
	if (parts[1][0] != '/')
		throw std::runtime_error("Error : Invalid Direction, location");
	else if (parts[2][0] != '{' || parts[2][1])
		throw std::runtime_error("Error : 3 ARgument, only '{' accepted, location");
	else if (op == true)
		throw std::runtime_error("Error : Two '{', location");
	else
		op = true;
}

void	Location::initPart(std::string& fileContent)
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

void	Location::initContrpart(std::string& fileContent)
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

void	Location::initAutoindex(std::string& fileContent)
{
	if (autoindex == true)
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
		verifauto = true;
		autoindex = false;
	}
}

void Location::initError_page(std::string& fileContent)
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
	for (size_t i = 0; i < error_codes.size(); i++)
	{
    error_page[error_codes[i]] = name;
	}

	if (name.empty())
		throw std::runtime_error("Error : Missing direction, error_page");
	// std::cout << "Error Page Mapping:\n";
	// for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); ++it)
	// {
	// 	std::cout << "Error Code: " << it->first << " -> File: " << it->second << std::endl;
	// }
}

bool Location::getOp() const {
    return op;
}

std::string Location::getRoot() const {
    return root;
}

std::string Location::getIndex() const {
    return index;
}

std::vector<std::string> Location::getAllowMethods() const {
    return allow_methods;
}

bool Location::getAutoindex() const {
    return autoindex;
}

bool Location::getVerifauto() const {
    return verifauto;
}

int Location::getClientMaxBodySize() const {
    return client_max_body_size;
}

std::string Location::getRetur() const {
    return retur;
}

std::vector<std::string> Location::getCgiPath() const {
    return cgi_path;
}

MyVector<std::string> Location::getCgiExt() const {
    return cgi_ext;
}

std::map<int, std::string> Location::getErrorPage() const
{
    return error_page;
}
