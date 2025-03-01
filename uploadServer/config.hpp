/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:54:40 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/16 10:48:55 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	config_hpp
	#define	config_hpp

	#include <iostream>
	#include <map>
	#include <vector>
	#include <sstream>
	#include <cstdlib>
	#include <string>
	#include <climits>
	#include <fstream>
	#include "Location.hpp"

		/* COLOR */
	#define RED "\033[31m"
	#define ORANGE "\033[38;5;208m"
	#define GREEN "\033[32m"
	#define BLUE "\033[34m"
	#define RESET "\033[0m"
	#define BOLD "\033[1m"
	#define UNDERLINE "\033[4m"
	
class Location;

class config
{
    public:
		config();
		config &operator=(const config &other);
		config(const config &other);
		virtual ~config();
		bool getOp() const;
		
    	std::vector<int> 						getPort() const;
    	std::string 							getConfigName() const;
    	std::string 							getHost() const;
    	std::string 							getRoot() const;
   		std::string 							getIndex() const;
		std::string 							getRetur() const;
    	std::map<int, std::string>				getErrorPage() const;
    	bool 									getAutoindex() const;
    	bool 									getVerifauto() const;
    	int 									getClientMaxBodySize() const;

		void	initconfig(std::string& fileContent);
		void 	parseLocations(const std::vector<std::string>& lines, std::string& current);
		void	parsconfig(std::string& fileContent, std::map<std::string, Location>& location, std::string& current);
		void	initPort(std::string& fileContent);
		void	initconfigName(std::string& fileContent);
		void	initHost(std::string& fileContent);
		void	initReturn(std::string& fileContent);
		void	initRoot(std::string& fileContent);
		void	initIndex(std::string& fileContent);
		void	initError_page(std::string& fileContent);
		void	initAutoindex(std::string& fileContent);
		void	initClient(std::string& fileContent);
		void	initPart(std::string& fileContent);
		void	initContrpart(std::string& fileContent);

		bool									op;
		std::vector<int>						port;
		std::string								config_name;
		std::string								host;
		std::string								root;
		std::string								index;
		std::string								retur;
		std::map<int, std::string>				error_page;
		bool									autoindex;
		bool									verifauto;
		int										client_max_body_size;
		std::map<std::string, Location>			location;
		
    protected:
};

inline int countWords(const std::string& str) {
    std::istringstream stream(str);
    std::string word;
    int wordCount = 0;

    while (stream >> word) {
        wordCount++;
    }
    
    return wordCount;
}

inline std::vector<std::string> splitt(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));

    return tokens;
}

std::vector<std::string> cut_conf_serv(const int argc, char *configFile);


# endif

