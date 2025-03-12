#include "../../../includes/Webserv.h"

void RequestIn::checkErrorHTTPHeaders( void ) {
    if (this->vector.size() != 3) {
        this->codeHTTP = 400;
        std::cerr << "B1" <<std::endl;
        return ;
    }

    if (this->mapParse.find("Host") == this->mapParse.end()) {
        this->codeHTTP = 400;
        std::cerr << "B2" <<std::endl;
        return ;
    }
    this->method = this->vector[0];
    this->uri = this->vector[1];
    this->protocol = this->vector[2];
    std:: cerr << "JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ: " <<  removeUriFile(this->uri, *this) << std::endl;
    if (this->serv->location.find(removeUriFile(this->uri, *this)) != this->serv->location.end())
    {
        this->loc = this->serv->location[removeUriFile(this->uri, *this)];
        this->cl.setGoodLocation(removeUriFile(this->uri, *this));
        this->locSet = true;
        std::cerr << "A" << std::endl;
    }
    else if (this->serv->location.find(removeUriFile(this->uri + "/", *this)) != this->serv->location.end())
    {
        this->loc = this->serv->location[removeUriFile(this->uri + "/", *this)];
        this->cl.setGoodLocation(removeUriFile(this->uri + "/", *this));
        this->locSet = true;
        std::cerr << "B" << std::endl;
    }
    else if (this->serv->location.find(removeUriFile(this->uri.substr(0, this->uri.length() - 1), *this)) != this->serv->location.end()) {
        this->loc = this->serv->location[removeUriFile(this->uri.substr(0, this->uri.length() - 1), *this)];
        this->cl.setGoodLocation(removeUriFile(this->uri.substr(0, this->uri.length() - 1), *this));
        this->locSet = true;
        std::cerr << "C" << std::endl;
    }
    std::vector<std::string> validMethod;
    std::cerr << "--------------------------------------------------------------------------------------------------------------------\n";
    for (std::map<std::string, Location>::iterator it = this->monitor.servors[3]->location.begin(); it != this->monitor.servors[3]->location.end(); ++it) {
		std::cerr << it->first << " : " << it->second.getVerifauto() << std::endl;
	}
    std::cerr << "--------------------------------------------------------------------------------------------------------------------\n";
    if (this->locSet)
        std::cerr << this->loc.getAutoindex() << std::endl;
    if (this->locSet)
        validMethod = this->loc.getAllowMethods();
    if (validMethod.empty()) {
        validMethod.push_back("GET");
        validMethod.push_back("POST");
        validMethod.push_back("DELETE");
    }
    if (this->cl.getTimeoutBool() && this->cl.getStatusCgi())
    {
        this->codeHTTP = 504;
        return ;
    }
    if (this->cl.getTimeoutBool())
    {
        this->codeHTTP = 408;
        return ;
    }
    if (std::find(validMethod.begin(), validMethod.end(), method) == validMethod.end()) {
        this->codeHTTP = 405;
        return ;
    }

    /* Check du path */
    if (uri[0] != '/') {
        this->codeHTTP = 400;
        std::cerr << "B3" <<std::endl;
        return ;
    }

    /* Check du protocole */
    if (protocol != "HTTP/1.1") {
        this->codeHTTP = 505;
        return ;
    }

    /* Check du corps de la requete (POST) */
    MyVector<std::string> mimeAccepted;
    mimeAccepted.push_back("multipart/form-data");
    mimeAccepted.push_back("application/x-www-form-urlencoded");
    mimeAccepted.push_back("text/plain");

    if (this->mapParse.find("Content-Length") != this->mapParse.end()) {
        std::istringstream stream2(this->mapParse["Content-Length"]);
        stream2 >> this->length;
        // std::cerr << "---> "<< this->length << std::endl;
        if (Checker::compNb(this->mapParse["Content-Length"]) == 0)
        {
            this->codeHTTP = 400;
            std::cerr << "B4" <<std::endl;
            return ;
        }

        if (this->length <= 0) {
            this->codeHTTP = 411;
            return ;
        }
        int lenP = -1;
        if (this->locSet)
            lenP = this->loc.getClientMaxBodySize();
        if (lenP < 0) {
            lenP = this->serv->getClientMaxBodySize();
        }
        if (lenP < 0)
            lenP = 1048576;
        if (this->length > lenP) {
            this->codeHTTP = 413;
            return ;
        }
    }

    if (this->method == "POST") {
        if (this->mapParse.find("Content-Length") == this->mapParse.end() && this->mapParse.find("Transfer-Encoding") == this->mapParse.end()) {
            this->codeHTTP = 411;
            return ;
        }
        else if (this->mapParse.find("Content-Length") != this->mapParse.end()) {
            std::istringstream stream(this->mapParse["Content-Length"]);
            stream >> this->length;
            // std::cerr << "---> "<< this->length << std::endl;

            if (this->length <= 0) {
                this->codeHTTP = 411;
                return ;
            }
        }
        else {
            std::istringstream streamBody(this->body);
            std::string newbdy = "";
            std::string stringBody;
            int totalLength = 0;
            int lenChunk;
            char c;

            do {
                std::getline(streamBody, stringBody);
                if (!stringBody.empty()) {
                    if ((lenChunk = Conversion::stringToInt(stringBody)) <= 0) {
                        this->codeHTTP = 400;
                        std::cerr << "B5" <<std::endl;
                        return ;
                    }
                    for (int k = 0; k < lenChunk && streamBody >> std::noskipws >> c; k++) {
                        newbdy += c;
                    }
                    totalLength += lenChunk;
                    if (static_cast<int>(newbdy.size()) != totalLength) {
                        this->codeHTTP = 400;
                        std::cerr << "B6" <<std::endl;
                        return ;
                    }
                }
            } while (!(stringBody.empty()));
            this->body = newbdy;
        }
        int len = -1;
        if (this->locSet)
            len = this->loc.getClientMaxBodySize();
        if (len < 0) {
            len = this->serv->getClientMaxBodySize();
        }
        if (len < 0) {
            len = 1000000;
        }
        if (this->length > len) {
            this->codeHTTP = 413;
            return ;
        }

        if (this->mapParse.find("Content-Type") == this->mapParse.end()) {
            this->codeHTTP = 400;
            std::cerr << "B7" <<std::endl;
            return ;
        }

        std::istringstream streamType(this->mapParse["Content-Type"]);
        streamType >> this->typeRequestGen;
        MyVector<std::string> vectorAccepted(this->typeRequestGen, ','); 

        // std::cerr << ORANGE << this->mapParse["Content-Type"]<<RESET << std::endl;
        std::string chEq = vectorAccepted(mimeAccepted);
        this->boundary = GenericGetter::postGetBoundary(this->mapParse["Content-Type"]);
        if (chEq == "NULL") {
            this->codeHTTP = 415;
            return ;
        }
    }
    if ((this->mapParse.find("Cookie") == this->mapParse.end() && (this->uri.find("/conect") != std::string::npos)) || (this->uri.find("/conect") != std::string::npos && this->mapParse.find("Cookie") != this->mapParse.end() && this->monitor.mapCookie.find(split(this->mapParse["Cookie"], '=')[1]) == this->monitor.mapCookie.end())) {
        this->codeHTTP = 401;
        return ;
    }
    if (this->mapParse.find("Cookie") != this->mapParse.end()) {
        this->sessionId = this->mapParse["Cookie"].substr(this->mapParse["Cookie"].find("SessionID=") + 12, this->mapParse["Cookie"].substr(this->mapParse["Cookie"].find("SessionID=") + 12).find("\r\n"));
    }
    else
        this->sessionId = "";
    this->codeHTTP = 200;
    return ;
}

void RequestIn::checkErrorHTTPBody( void ) {
	
    if (this->length != static_cast<int>(this->body.size())) {
        this->codeHTTP = 400;
        std::cerr << "B8" <<std::endl;
        return ;
    }
    
    MyVector<std::string> mimeAccepted;
    mimeAccepted.push_back("text/html");
    mimeAccepted.push_back("text/css");
    mimeAccepted.push_back("application/javascript");
    mimeAccepted.push_back("image/apng");
    mimeAccepted.push_back("image/jpeg");
    mimeAccepted.push_back("application/json");
    mimeAccepted.push_back("*/*");
    mimeAccepted.push_back("text/plain");

    if (this->mapParse.find("Accept") != this->mapParse.end()) {
        std::istringstream streamType(this->mapParse["Accept"]);
        std::string typeRequest;
        streamType >> typeRequest;
        MyVector<std::string> vectorAccepted(typeRequest, ','); 

        // std::cerr << this->mapParse.find("Accept")->second << std::endl;
        std::string chEq = vectorAccepted(mimeAccepted);
        if (chEq == "NULL") {
            this->codeHTTP = 406;
            return ;
        }
        else
            this->mimeAccept = chEq;
    }

    this->codeHTTP = 200;
    return ;

}

void RequestIn::parseBody( void ) {
    if (this->mapParse.find("Content-Type") != this->mapParse.end()) {
        if (this->mapParse["Content-Type"] == " application/json") {
            this->codeHTTP = Checker::parseJson(this->body);
            std::cout << "JSON" << std::endl;
            return ;
        }
    }
}
