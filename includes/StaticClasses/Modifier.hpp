#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "../Webserv.h"

#include <sys/stat.h>
#include <dirent.h>

class Modifier {
    private:
        Modifier();
        Modifier(Modifier& cpy);
        Modifier& operator=(Modifier& cpy);
        ~Modifier();

    public:
        static void modifyMapCookies(monitoring& monitor, std::string& response);
};
