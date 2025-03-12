#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

#include "Utils.hpp"
#include "Cookie.hpp"
#include "config.hpp"
#include "client.hpp"
#include "serveur.hpp"
#include "Location.hpp"
#include "Parser.hpp"

#include "StaticClasses/Conversion.hpp"
#include "StaticClasses/Checkers.hpp"
#include "StaticClasses/Initer.hpp"
#include "StaticClasses/GenericGetter.hpp"
#include "StaticClasses/Modifier.hpp"

#ifndef BEFORE
# define BEFORE 1
#endif

#ifndef AFTER
# define AFTER 2
#endif

class RequestIn;

std::string checkRedir(RequestIn& req, int n);
std::vector<std::string> makeAutoIndex( RequestIn& req );
std::vector<std::string> split(const std::string& str, char delimiter);
MyStack<std::string> create1Stack(const std::string& ch);
void addElemToStr(std::string& str, MyStack<std::string>& stack);
MyTriple<std::string, std::string, bool>  makeFilePost(RequestIn& input);
void createFile(MyTriple<std::string, std::string, bool>& triple, RequestIn& req);
std::string removeUriFile(std::string uri, RequestIn& req);
std::string timeout(client& cl, monitoring& monitor, std::string code, std::map<int, std::string> map);
void	delete_client(monitoring &moni, int fd, int i);
char *	my_strdup(const std::string &str);
