#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "../Utils.hpp"
#include "../Webserv.h"

#include <sys/stat.h>
#include <dirent.h>

class Initer {
    private:
        Initer();
        Initer(Initer& cpy);
        Initer& operator=(Initer& cpy);
        ~Initer();

    public:
        static std::map<int, std::string> initCodeMap( void );
        static std::string loadPage(std::string& catFile);
        static std::string generateSessionId();
        static std::string makeTheSample(std::string code, std::string str, std::string sample);
        static void		creat_servor(std::vector<std::string> &cut_str_serv, monitoring &moni);
        static serveur *malloc_serv(std::string &str);
};
