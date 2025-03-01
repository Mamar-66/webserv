#include "../../../includes/Parser.hpp"

std::vector<std::string> RequestIn::PushResponse( void /* ParseConfig& config */ ) {
    std::map<int, std::string> mapCode = Initer::initCodeMap();
    std::string rootedDir = GenericGetter::findRoot(*this); // CHANGE WITH CONF
    std::vector<std::string> vectorElems;
    std::string sample201 = "./html/conect/utilsSample/uploadSucces.html";
    std::string catFile = rootedDir + this->uri;

    std::map<int, std::string> mapCodeHtml = this->serv->getErrorPage();
    std::map<int, std::string> mapCodeLocation = this->loc.getErrorPage();
    for (std::map<int, std::string>::iterator it = mapCodeLocation.begin(); it != mapCodeLocation.end(); it++)
        mapCodeHtml[it->first] = it->second;
    
    if (this->codeHTTP - 400 < 0) {
        MyTriple<std::string, std::string, bool> fileToSave = makeFilePost(*this);
        std::cout << ORANGE << (Checker::isFile(catFile.substr(0, catFile.find_last_of('?'))) && this->loc.getCgiExt() == catFile.substr(0, catFile.find_last_of('?')).substr(catFile.find_last_of('.')) && access(catFile.substr(0, catFile.find_last_of('?')).c_str(), X_OK) == 0) << std::endl;
        if (Checker::isFile(catFile.substr(0, catFile.find_last_of('?'))) && this->loc.getCgiExt() == catFile.substr(0, catFile.find_last_of('?')).substr(catFile.find_last_of('.')) && access(catFile.substr(0, catFile.find_last_of('?')).c_str(), X_OK) == 0)  {
            return this->holdCGI(mapCodeHtml);
        }
        createFile(fileToSave);

        if (fileToSave.third == false)
            this->codeHTTP = 201;
        else
            this->codeHTTP = 204;
        this->stringCode = mapCode[this->codeHTTP];
        

        std::string htmlResponse;
        if (this->codeHTTP == 201) {
            htmlResponse = Initer::loadPage(sample201);
            std::cout << GREEN << htmlResponse << RESET << std::endl;
        }
        else
            htmlResponse = "";
        vectorElems.push_back("HTTP/1.1 ");
        vectorElems.push_back(Conversion::intToString(this->codeHTTP));
        vectorElems.push_back(" ");
        vectorElems.push_back(this->stringCode);
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Connexion: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(GenericGetter::getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
    }
    else {
        std::map<int, std::string> mapCodeHtml = this->serv->getErrorPage();
        std::map<int, std::string> mapCodeLocation = this->loc.getErrorPage();
        for (std::map<int, std::string>::iterator it = mapCodeLocation.begin(); it != mapCodeLocation.end(); it++)
            mapCodeHtml[it->first] = it->second;
        MyVector<int> vectorCodeGenerate;

        for (std::map<int, std::string>::iterator it = mapCodeHtml.begin(); it != mapCodeHtml.end(); ++it) {
            vectorCodeGenerate.push_back(it->first);
        }
        std::string htmlResponse;
        if (vectorCodeGenerate == this->codeHTTP)
            htmlResponse = Initer::loadPage(mapCodeHtml[this->codeHTTP]);
        else
            htmlResponse = "";


        vectorElems.push_back("HTTP/1.1 ");
        vectorElems.push_back(Conversion::intToString(this->codeHTTP));
        vectorElems.push_back(" ");
        vectorElems.push_back(this->stringCode);
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Connection: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(GenericGetter::getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
    }
    return vectorElems;
}
