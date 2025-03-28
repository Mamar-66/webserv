#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <map>

#include "Webserv.h"

class Cookie {
    private:


        std::string userID;
        std::string sessionID;
        std::string username;
        long timestampLastUsed;

    public:
        /* Constructors */
        Cookie();
        Cookie(std::string u, std::string s, std::string usr);
        ~Cookie();
        Cookie(const Cookie& cpy);
        Cookie& operator=(const Cookie& cpy);

        /* Setters */
        void setUserID(std::string id);
        void setSessionID(std::string id);
        void setTimestamp(long newTs);

        /* Checkers */
        bool isSameUserId(std::string id);
        bool isSameSessionId(std::string id);
        bool isSessionStillUp(long timestamp, long timeToExpire);

        /* Getters */
        std::string& getUserID( void );
        std::string& getSessionID( void );
        std::string& getUsername( void );
        long& getTimestamp( void );

        std::string getRepertory( void );
};
