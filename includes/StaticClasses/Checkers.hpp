#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "../Webserv.h"

#include <sys/stat.h>
#include <dirent.h>

class Checker {
	private:
		Checker();
		~Checker();
		Checker(Checker& cpy);
		Checker& operator=(Checker& cpy);
	
	public:
		static bool isDirectory(const std::string& path);
		static bool isFile(const std::string& path);
		static int compNb(std::string s);
		static int AnalyseStacks(std::stack<char> elems, std::stack<MyTriple<char, char, char> > coma);
		static int analyseSub(std::string body, int i);
		static int parseJson(std::string& body);
		static int parseCatFile(std::string f, RequestIn& req);
		static bool time_out(monitoring &moni, int fd, int i);
		static bool	if_close_pipe(monitoring &moni, int fd, int i);
		static bool 		compar(const int fd, const std::vector<pollfd> &poll_servor);
};
