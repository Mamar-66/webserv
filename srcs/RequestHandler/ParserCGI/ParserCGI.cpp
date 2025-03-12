#include "../../../includes/Webserv.h"

std::vector<std::string> RequestIn::holdCGI( std::map<int, std::string> mapCodeHtml ) {
    std::vector<std::string> vectorElems;

    char **env = creatEnv(*this, mapCodeHtml);

    this->cl.setEnv(env);
    if (start_CGI(this->monitor, this->cl) == false){
        this->codeHTTP = 500;
        return this->GetResponse();
    }
    vectorElems.push_back(this->body);
    return vectorElems;
}
