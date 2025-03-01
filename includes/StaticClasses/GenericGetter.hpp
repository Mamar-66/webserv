#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "../Utils.hpp"
#include "../Webserv.h"
#include "../Parser.hpp"
#include <sys/stat.h>
#include <dirent.h>

class GenericGetter {
    private:
        GenericGetter();
        GenericGetter(GenericGetter& cpy);
        GenericGetter& operator=(GenericGetter& cpy);
        ~GenericGetter();

    public:
        static std::string getHttpDate();
        static std::string postGetBoundary(std::string& str);
        static std::string getLittleInBig(std::string& input, std::string& delim, std::string& deli2);
        static std::string findRoot(RequestIn& req);
        static char removeSpaces(std::string body, int cas, int i);
};
