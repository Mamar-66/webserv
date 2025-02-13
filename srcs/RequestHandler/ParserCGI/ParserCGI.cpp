#include "../../../includes/Parser.hpp"


std::string RequestIn::holdCGI( void ) {
    int pipefd[2];
    
    if ( pipe(pipefd)  == -1 )  {
        this->codeHTTP = 500;
        return "";
    }

    pid_t pid = fork();

    if (pid < 0) {
        this->codeHTTP = 500;
        return "";
    }
    else if (pid == 0) {
        return "";/*
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        std::string serverName = "SERVER_NAME=" + "myWebserv.com"; // Change "myWebserv.com" to a #DEFINE variable
        std::string serverProtocol = "HTTP/1.1";
        std::string serverPort = static_cast<std::string>(this->port); //change to parserClass
        std::string scriptName = "kkk"

        this->envVector.push_back();*/
    }
    else 
        return "";
    
}
