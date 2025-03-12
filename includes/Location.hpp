/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:19:22 by sbarbe            #+#    #+#             */
/*   Updated: 2025/02/21 22:59:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LOCATION_HPP
	#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <map>
#include "Webserv.h"

class Location
{
    public:
		Location();
		Location &operator=(const Location &other);
		Location(const Location &other);
		virtual ~Location();
		
		bool 						getOp() const;
    	std::string					getRoot() const;
    	std::string 				getIndex() const;
    	std::vector<std::string> 	getAllowMethods() const;
    	bool 						getAutoindex() const;
    	bool 						getVerifauto() const;
    	int 						getClientMaxBodySize() const;
		std::map<int, std::string>	getErrorPage() const;
    	std::string 				getRetur() const;
    	MyVector<std::string>	 	getCgiExt() const;

		void	initRoot(std::string& fileContent);
		void	initIndex(std::string& fileContent);
		void	initAllow_methods(std::string& fileContent);
		void	initClient(std::string& fileContent);
		void	initReturn(std::string& fileContent);
		void	initCgi_ext(std::string& fileContent);
		void	initLocation(std::string& fileContent);
		void	initError_page(std::string& fileContent);
		void	initPart(std::string& fileContent);
		void	initContrpart(std::string& fileContent);
		void	initAutoindex(std::string& fileContent);

		bool						op;
		std::string					root;
		std::string					index;
		std::map<int, std::string>	error_page;
		std::vector<std::string>	allow_methods;
		bool						autoindex;
		bool						verifauto;
		int							client_max_body_size;
		std::string					retur;
		MyVector<std::string>		cgi_ext;	
		bool 						passwordDependent;
    protected:
};

std::vector<std::string> splitLines(const std::string &input);
std::string normalizeSpaces(const std::string &input);
void	parsconfigL(std::string& fileContent, std::map<std::string, Location>& location, std::string& current);

# endif
