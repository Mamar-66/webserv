/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbarbe <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:19:22 by sbarbe            #+#    #+#             */
/*   Updated: 2025/01/16 15:19:24 by sbarbe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LOCATION_HPP
	#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <map>
#include "config.hpp"

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
    	std::string 				getRetur() const;
    	std::vector<std::string> 	getCgiPath() const;
    	std::vector<std::string> 	getCgiExt() const;

		void	initRoot(std::string& fileContent);
		void	initIndex(std::string& fileContent);
		void	initAllow_methods(std::string& fileContent);
		void	initClient(std::string& fileContent);
		void	initReturn(std::string& fileContent);
		void	initCgi_path(std::string& fileContent);
		void	initCgi_ext(std::string& fileContent);
		void	initLocation(std::string& fileContent);
		void	initPart(std::string& fileContent);
		void	initContrpart(std::string& fileContent);
		void	initAutoindex(std::string& fileContent);

		bool						op;
		std::string					root;
		std::string					index;
		std::vector<std::string>	allow_methods;
		bool						autoindex;
		bool						verifauto;
		int							client_max_body_size;
		std::string					retur;
		std::vector<std::string>	cgi_path;
		std::vector<std::string>	cgi_ext;	
    protected:
};

std::map<std::string, Location> parseLocations(const std::vector<std::string>& lines, std::string& current);
std::vector<std::string> splitLines(const std::string &input);
std::string normalizeSpaces(const std::string &input);
void	parsconfigL(std::string& fileContent, std::map<std::string, Location>& location, std::string& current);

# endif
