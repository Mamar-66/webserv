#include "../../../includes/Parser.hpp"

void RequestIn::checkErrorHTTPHeaders( void /* ParseConfig& config */) {
    // myVector<std::string> vector;
    if (this->vector.size() != 3) {
        this->codeHTTP = 400;
        return ;
    }

    if (this->mapParse.find("Host") == this->mapParse.end()) {
        this->codeHTTP = 400;
        return ;
    }
    this->method = this->vector[0];
    this->uri = this->vector[1];
    this->protocol = this->vector[2];
    std:: cout << "JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ: " <<  removeUriFile(this->uri, *this) << std::endl;
    this->loc = serv->location[removeUriFile(this->uri, *this)];
    this->cl.setGoodLocation(removeUriFile(this->uri, *this));
    // std::cout << this->loc.getCgiExt()[0] << std::endl;
    std::vector<std::string> validMethod = this->loc.getAllowMethods();
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
        return ;
    }

    // if (uri.size() > 999999) { //config.getMaxSizeUri()
    //     this->codeHTTP = 414;
    //     return ;
    // }

    /* Check du protocole */
    if (protocol != "HTTP/1.1") {
        this->codeHTTP = 505;
        return ;
    }

    /* Check du corps de la requete (POST) */
    MyVector<std::string> mimeAccepted;
    mimeAccepted.push_back("multipart/form-data"); // EN BRUT RAJOUTER POUR CGI;
    mimeAccepted.push_back("application/x-www-form-urlencoded");

    if (this->mapParse.find("Content-Length") != this->mapParse.end()) {
        std::istringstream stream2(this->mapParse["Content-Length"]);
        stream2 >> this->length;
        std::cout << "---> "<< this->length << std::endl;
        if (Checker::compNb(this->mapParse["Content-Length"]) == 0)
        {
            this->codeHTTP = 400;
            return ;
        }

        if (this->length <= 0) {
            this->codeHTTP = 411;
            return ;
        }
        int lenP = this->loc.getClientMaxBodySize();
        if (lenP < 0) {
            lenP = this->serv->getClientMaxBodySize();
        }
        if (this->length > lenP) { //config.getmaxLength()
            this->codeHTTP = 413;
            return ;
        }
    }

    if (this->method == "POST") { //Change to body-accept request
        if (this->mapParse.find("Content-Length") == this->mapParse.end() && this->mapParse.find("Transfer-Encoding") == this->mapParse.end()) {
            this->codeHTTP = 411;
            return ;
        }
        else if (this->mapParse.find("Content-Length") != this->mapParse.end()) {
            std::istringstream stream(this->mapParse["Content-Length"]);
            stream >> this->length;
            std::cout << "---> "<< this->length << std::endl;

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
                        return ;
                    }
                    for (int k = 0; k < lenChunk && streamBody >> std::noskipws >> c; k++) {
                        newbdy += c;
                    }
                    totalLength += lenChunk;
                    if (static_cast<int>(newbdy.size()) != totalLength) {
                        this->codeHTTP = 400;
                        return ;
                    }
                }
            } while (!(stringBody.empty()));
            this->body = newbdy;
        }
        int len = this->loc.getClientMaxBodySize();
        if (len < 0) {
            len = this->serv->getClientMaxBodySize();
        }
        if (this->length > len) { //config.getmaxLength()
            this->codeHTTP = 413;
            return ;
        }

        if (this->mapParse.find("Content-Type") == this->mapParse.end()) {
            this->codeHTTP = 400;
            return ;
        }

        std::istringstream streamType(this->mapParse["Content-Type"]);
        std::string typeRequest;
        streamType >> typeRequest;
        MyVector<std::string> vectorAccepted(typeRequest, ','); 

        std::cout << this->mapParse.find("Content-Type")->second << std::endl;
        std::string chEq = vectorAccepted(mimeAccepted);
        this->boundary = GenericGetter::postGetBoundary(this->mapParse["Content-Type"]);
        if (chEq == "NULL") {
            this->codeHTTP = 415;
            return ;
        }
    }
    if ((this->mapParse.find("Cookie") == this->mapParse.end() && (this->uri.find("/conect") != std::string::npos)) || (this->uri.find("/conect") != std::string::npos && this->mapParse.find("Cookie") != this->mapParse.end() && this->monitor.mapCookie.find(this->mapParse["Cookie"].substr(1)) == this->monitor.mapCookie.end())) {
        this->codeHTTP = 401;
        return ;
    }
    // else
    //     std::cout << this->monitor.mapCookie[this->mapParse["Cookie"]].getSessionID() << " :-: " << this->monitor.mapCookie[this->mapParse["Cookie"]].getUserID() << std::endl;
    this->codeHTTP = 200;
    return ;
}

void RequestIn::checkErrorHTTPBody( void /* ParseConfig& config */) {
	
    if (this->length != static_cast<int>(this->body.size())) {
        this->codeHTTP = 400;
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
    mimeAccepted.push_back("text/plain") ; // config.getMimes();

    if (this->mapParse.find("Accept") != this->mapParse.end()) {
        std::istringstream streamType(this->mapParse["Accept"]);
        std::string typeRequest;
        streamType >> typeRequest;
        MyVector<std::string> vectorAccepted(typeRequest, ','); 

        // std::cout << this->mapParse.find("Accept")->second << std::endl;
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
        /*if (this->mapParse["Content-Type"] == "html" || this->mapParse["Content-Type"] == "htm") {
            this->codeHTTP = parseHtml(this->body);
            return ;
        }
        if (this->)*/
    }
}
