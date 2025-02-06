/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:54:40 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/24 12:06:51 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>
#include <climits>
#include <sstream>

class config
{
    public:
		config();
		config &operator=(const config &other);
		config(const config &other);
		virtual ~config();
		void	getconfig(std::string& fileContent);
		void	getPort(std::string& fileContent);
		void	getconfigName(std::string& fileContent);
		void	getHost(std::string& fileContent);
		void	getRoot(std::string& fileContent);
		void	getIndex(std::string& fileContent);
		void	initError_page(std::string& fileContent);
		void	initAutoindex(std::string& fileContent);
		void	initClient(std::string& fileContent);
		void	initPart(std::string& fileContent);
		void	initContrpart(std::string& fileContent);
		bool						op;
		int							port;
		std::string	config_name;
		std::vector<std::string>	host;
		std::string	root;
		std::string	index;
		std::vector<std::string>	error_page;
		std::string	autoindex;
		int			client_max_body_size;
    private:
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

inline std::vector<std::string> split(const std::string &str, char delimiter) {
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
