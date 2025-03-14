#include "../../../includes/Webserv.h"
#include <fstream>
#include <iostream>

std::vector<std::string> RequestIn::PushResponse( void ) {
    std::map<int, std::string> mapCode = Initer::initCodeMap();
    std::string rootedDir = GenericGetter::findRoot(*this);
    std::vector<std::string> vectorElems;
    std::string sample201 = "./html/conect/utilsSample/uploadSucces.html";
    std::string sample201T = "./html/conect/utilsSample/commentSuccess.html";
    std::string catFile = rootedDir + this->uri;
    std::string ComPage = "./html/conect/commentaires.txt";
    bool type = false;
    std::fstream file(ComPage.c_str(), std::ios::out | std::ios::app);
    std::map<int, std::string> mapCodeHtml = Initer::initMapConfig(this->monitor, this->cl.getFD());

    if (!(file.is_open())) 
        this->codeHTTP = 500;
    
    if (this->codeHTTP - 400 < 0) {
        if (this->typeRequestGen != "multipart/form-data;") {
            if (Checker::isFile(catFile.substr(0, catFile.find_last_of('?'))) && this->locSet && this->loc.getCgiExt() == catFile.substr(0, catFile.find_last_of('?')).substr(catFile.find_last_of('.')) && access(catFile.substr(0, catFile.find_last_of('?')).c_str(), X_OK) == 0)  {
                return this->holdCGI(mapCodeHtml);
            }
            if (this->mapParse["Content-Type"] == " plain/text") {
                file << this->monitor.mapCookie[this->sessionId].getUsername() << " has posted a comment on " << GenericGetter::getHttpDate() << ":\n" << this->body << "\n\n";
                type = true;
            }
            if (this->locSet && this->loc.getCgiExt() == catFile.substr(0, catFile.find_last_of('?')).substr(catFile.find_last_of('.')))
            {
                this->codeHTTP = 404;
                return this->GetResponse();
            }
            else {
                this->codeHTTP = 204;
                this->stringCode = mapCode[this->codeHTTP];
                std::vector<std::string> vectoNoChanges;
                vectoNoChanges.push_back("HTTP/1.1 ");
                vectoNoChanges.push_back(Conversion::intToString(this->codeHTTP));
                vectoNoChanges.push_back(" ");
                vectoNoChanges.push_back(this->stringCode);
                vectoNoChanges.push_back("\r\n");
                vectoNoChanges.push_back("Content-Type: text/html\r\n");
                vectoNoChanges.push_back("Content-Length: ");
                vectoNoChanges.push_back("0");
                vectoNoChanges.push_back("\r\n");
                vectoNoChanges.push_back("Connexion: ");
                vectoNoChanges.push_back("close\r\n");
                vectoNoChanges.push_back("Date: ");
                vectoNoChanges.push_back(GenericGetter::getHttpDate());
                vectoNoChanges.push_back("\r\n\r\n");
                return vectoNoChanges;
            }
        }
        MyTriple<std::string, std::string, bool> fileToSave = makeFilePost(*this);
        createFile(fileToSave, *this);

        if (fileToSave.third == false || type == true)
            this->codeHTTP = 201;
        else if (type == false)
            this->codeHTTP = 204;
        this->stringCode = mapCode[this->codeHTTP];


        std::string htmlResponse;
        if (this->codeHTTP == 201) {
            if (type)
                htmlResponse = Initer::loadPage(sample201T);
            else
                htmlResponse = Initer::loadPage(sample201);
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
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size())));
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Connexion: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(GenericGetter::getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
    }
    else {
        std::map<int, std::string> mapCodeLocation;
        std::map<int, std::string> mapCodeHtml = this->serv->getErrorPage();
        if (this->locSet)
            mapCodeLocation = this->loc.getErrorPage();
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
            htmlResponse = Initer::makeTheSample(Conversion::intToString(this->codeHTTP), mapCode[this->codeHTTP], "./html/conect/errors/errors_sample/error_sample.html");

        vectorElems.push_back("HTTP/1.1 ");
        vectorElems.push_back(Conversion::intToString(this->codeHTTP));
        vectorElems.push_back(" ");
        vectorElems.push_back(this->stringCode);
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size())));
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
