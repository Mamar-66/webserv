#include "../../../includes/Parser.hpp"

void RequestIn::checkErrorHTTPHeaders( void /* ParseConfig& config */) {
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

    std::vector<std::string> validMethod;
    validMethod.push_back("GET");
    validMethod.push_back("POST");
    //validMethod.push_back("PUT");

    validMethod.push_back("DELETE"); //config.getValidMethod() ;
    if (std::find(validMethod.begin(), validMethod.end(), method) == validMethod.end()) {
        this->codeHTTP = 405;
        return ;
    }

    /* Check du path */
    if (uri[0] != '/') {
        this->codeHTTP = 400;
        return ;
    }

    if (uri.size() > 999999) { //config.getMaxSizeUri()
        this->codeHTTP = 414;
        return ;
    }

    /* Check du protocole */
    if (protocol != "HTTP/1.1") {
        this->codeHTTP = 505;
        return ;
    }

    /* Check du corps de la requete (POST) */
    std::vector<std::string> mimeAccepted;
    mimeAccepted.push_back("text/html");
    mimeAccepted.push_back("text/css");
    mimeAccepted.push_back("application/javascript");
    mimeAccepted.push_back("image/apng");
    mimeAccepted.push_back("image/jpeg");
    mimeAccepted.push_back("application/json");
    mimeAccepted.push_back("text/plain") ; // config.getMimes();

    if (this->mapParse.find("Content-Length") != this->mapParse.end()) {
        std::istringstream stream2(this->mapParse["Content-Length"]);
        stream2 >> this->length;
        std::cout << "---> "<< this->length << std::endl;
        if (compNb(this->mapParse["Content-Length"]) == 0)
        {
            this->codeHTTP = 400;
            return ;
        }

        if (this->length <= 0) {
            this->codeHTTP = 411;
            return ;
        }

        if (this->length > 99999999) { //config.getmaxLength()
            this->codeHTTP = 413;
            return ;
        }
    }

    if (this->method == "POST") { //Change to body-accept request
        if (this->mapParse.find("Content-Length") == this->mapParse.end()) {
            this->codeHTTP = 411;
            return ;
        }
        std::istringstream stream(this->mapParse["Content-Length"]);
        stream >> this->length;
        std::cout << "---> "<< this->length << std::endl;

        if (this->length <= 0) {
            this->codeHTTP = 411;
            return ;
        }

        if (this->length > 99999999) { //config.getmaxLength()
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

        if (std::find(mimeAccepted.begin(), mimeAccepted.end(), typeRequest) == mimeAccepted.end()) {
            this->codeHTTP = 415;
            return ;
        }
    }
    this->codeHTTP = 200;
    return ;
}

void RequestIn::checkErrorHTTPBody( void /* ParseConfig& config */) {
    std::cout << this->length << " :: " << this->body << static_cast<int>(this->body.size()) << std::endl;
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

        std::cout << this->mapParse.find("Accept")->second << std::endl;
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
            this->codeHTTP = parseJson(this->body);
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
