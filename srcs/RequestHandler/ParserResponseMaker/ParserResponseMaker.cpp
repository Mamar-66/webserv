#include "../../../includes/Webserv.h"

std::vector<std::string> RequestIn::GetResponse( void ) {
    std::map<int, std::string> mapCode = Initer::initCodeMap();

    std::string rootedDir = GenericGetter::findRoot(*this);

    std::vector<std::string> vectorElems;

    std::string temp = checkRedir(*this, 0);
    if (temp != this->uri && temp != "" and this->codeHTTP != 310)
        this->codeHTTP = 307;
    if (temp != "")
        this->uri = temp;

    std::string catFile = rootedDir + this->uri;
    bool AutoIndexToRemove = false;
    if (this->locSet)
    {
        AutoIndexToRemove = this->loc.getAutoindex();
    }
    if (!(this->locSet) || (this->locSet && this->loc.verifauto == false)) {
        AutoIndexToRemove = this->serv->getAutoindex();
    }

    
    if (!(Checker::parseCatFile(catFile, *this)))
        this->codeHTTP = 403;
    if (this->codeHTTP == 200 || this->codeHTTP / 100 == 3) {
        if (Checker::isDirectory(catFile)) {
            if (catFile[catFile.size() - 1] != '/')
                catFile += "/";
            std::string defaultP = "";
            if (this->locSet)
                defaultP = this->loc.getIndex();
            if (defaultP.empty())
                defaultP = this->serv->getIndex();
            if (defaultP.empty())
                defaultP = "index.html";
            catFile = catFile + defaultP;
            if (!(Checker::isFile(catFile))) {
                if (AutoIndexToRemove == false)
                    this->codeHTTP = 403;
                else
                    return makeAutoIndex(*this);
            }

        }
    }
    if (this->codeHTTP == 200) {
        if (!(Checker::isFile(catFile))) {
            if (!(Checker::isFile(catFile.substr(0, catFile.find_last_of('?')))))
                this->codeHTTP = 404;
        }
    }

    this->stringCode = mapCode[this->codeHTTP];
    
    vectorElems.push_back("HTTP/1.1 ");
    vectorElems.push_back(Conversion::intToString(this->codeHTTP));
    vectorElems.push_back(" ");
    vectorElems.push_back(this->stringCode);
    vectorElems.push_back("\r\n");

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

    if (this->codeHTTP - 400 >= 0 || this->codeHTTP == 310) {
		
		std::string htmlResponse;
        if (vectorCodeGenerate == this->codeHTTP)
        {
            htmlResponse = Initer::loadPage(mapCodeHtml[this->codeHTTP]);
        }
        else
            htmlResponse = Initer::makeTheSample(Conversion::intToString(this->codeHTTP), this->stringCode, "./html/conect/errors/errors_sample/error_sample.html");


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
    else if (this->codeHTTP / 100 == 3) {
        vectorElems.push_back("Location: " + this->uri);
        vectorElems.push_back("\r\nConnection: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(GenericGetter::getHttpDate());
        vectorElems.push_back("\r\n\r\n");
    }
    else if (this->codeHTTP / 100 == 2) {
        if (Checker::isFile(catFile.substr(0, catFile.find_last_of('?'))) && this->locSet && this->loc.getCgiExt() == catFile.substr(0, catFile.find_last_of('?')).substr(catFile.substr(0, catFile.find_last_of('?')).find_last_of('.')) && access(catFile.substr(0, catFile.find_last_of('?')).c_str(), X_OK) == 0)
            return this->holdCGI(mapCodeHtml);
        std::string htmlResponse = Initer::loadPage(catFile);
        vectorElems.push_back("Content-Type: ");
        vectorElems.push_back(this->mimeAccept);
        vectorElems.push_back("\r\n");
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

std::string RequestIn::makeResponse( void ) {
    std::vector<std::string> vectorElems;

	if (this->method == "POST") {
		vectorElems = this->PushResponse();
	}
	else if (this->method == "DELETE") {
        vectorElems = this->DeleteResponse();
    }
    else if (this->method == "GET") {
        vectorElems = this->GetResponse();
    }
    else {
        this->codeHTTP = 501;
        vectorElems = this->GetResponse();
    }
    return Conversion::vectorToString(vectorElems);
}
