#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <map>

class Cookie {
    private:
        Cookie(Cookie& cpy);
        Cookie& operator=(Cookie& cpy);


        std::string userID;
        std::string sessionID;
        long timestampLastUsed;
};