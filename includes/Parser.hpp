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
#include <ctime>

#include <sys/stat.h>
#include <dirent.h>

#include "Utils.hpp"
#include "Webserv.h"
#include "serveur.hpp"

#ifndef BEFORE
# define BEFORE 1
#endif

#ifndef AFTER
# define AFTER 2
#endif

class RequestIn {
    private:
        std::map<std::string, std::string> mapParse;
        std::map<std::string, int> mapCount;
        std::vector<std::string> vector;
        int length;
        std::string method;
        std::string uri;
        std::string protocol;
        std::string body;
        std::string stringCode;
        std::string mimeAccept;
        std::vector<std::string> envVector;
        int codeHTTP;

        int clientRequest; // Move into client class

        int port; // Move into parserClass
        
        RequestIn(RequestIn& cpy);
        RequestIn& operator=(RequestIn& cpy);

    public:
        RequestIn(const std::string& request/* , char **envp */);
        ~RequestIn();

        void checkErrorHTTPHeaders( void );
        void checkErrorHTTPBody( void );
        void parseBody( void );
        std::string holdCGI( void );

        std::string getURI(void);
    
        std::map<std::string, std::string> getMap( void );

		std::string makeResponse(monitoring &moni, client &cl);
		std::string canKeepAlive( void );
        int getCode( void );

        std::vector<std::string> PushResponse( void /* ParseConfig& config */ );
        std::vector<std::string> GetResponse( void /* ParseConfig& config */ );
        std::vector<std::string> DeleteResponse();
        std::vector<std::string> PostResponse(const std::string& input);


        /* Getters */
        std::map<std::string, int> getMapCount( void );

        std::string getMethod( void );
        std::string getProtocol( void );

        int getLength( void );
        std::string getStringCode( void );
        std::string getBody( void );
        
        std::string getMimeAccept( void );
        std::vector<std::string> getVector( void );
        std::vector<std::string> getEnvVector( void );

		/* omar */
		void 	init_envp(client &cl);
};
