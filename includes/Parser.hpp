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
        int codeHTTP;
        std::string boundary;
        std::string sessionId;
        std::string typeRequestGen;

        bool locSet;
        monitoring& monitor;
        serveur* serv;
        Location  loc;
        client& cl;
        
        RequestIn(RequestIn& cpy);
        RequestIn& operator=(RequestIn& cpy);

    public:
        RequestIn(client& request, monitoring& monitor/* , char **envp */);
        ~RequestIn();

        void checkErrorHTTPHeaders( void );
        void checkErrorHTTPBody( void );
        void parseBody( void );
        std::vector<std::string> holdCGI(std::map<int, std::string> mapCodeHtml);

        std::string getURI(void);
    
        std::map<std::string, std::string> getMap( void );

		std::string makeResponse( void );
        int getCode( void );

        std::vector<std::string> PushResponse( void /* ParseConfig& config */ );
        std::vector<std::string> GetResponse( void /* ParseConfig& config */ );
        std::vector<std::string> DeleteResponse();


        /* Getters */
        std::map<std::string, int> getMapCount( void );

        int getLength( void );
        bool        &getLocSet( void );
        std::string getMethod( void );
        std::string getProtocol( void );
        std::string getStringCode( void );
        std::string& getBody( void );
        std::string getMimeAccept( void );
        std::string getBoundary( void );

        Location& getLoc( void );
        serveur* getServ( void );
        monitoring& getMonitor( void );

        std::vector<std::string> getVector( void );
        
        std::string& getsessionId( void );

        /* Setters */
        
        void setCode( int code );
        void setUri( std::string uri );
        void setLoc(Location& loc);
		/* omar */
};
        char **creatEnv(RequestIn& req, std::map<int, std::string> mapErrorHtml);
