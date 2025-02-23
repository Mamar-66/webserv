#include "../../includes/Parser.hpp"

std::string RequestIn::getURI( void ) {
    return this->uri;
}

std::map<std::string, std::string> RequestIn::getMap( void ) {
    return this->mapParse;
}

int RequestIn::getCode( void ) {
    return this->codeHTTP;
}


std::map<std::string, int> RequestIn::getMapCount( void ) {
    return this->mapCount;
}

std::vector<std::string> RequestIn::getVector( void ) {
    return this->vector;
}
int RequestIn::getLength( void ) {
    return this->length;
}


std::string RequestIn::getMethod( void ) {
    return this->method;
}

std::string RequestIn::getProtocol( void ) {
    return this->protocol;
}

std::string& RequestIn::getBody( void ) {
    return body;
}

std::string RequestIn::getStringCode( void ) {
    return stringCode;
}

std::string RequestIn::getMimeAccept( void ) {
    return mimeAccept;
}

std::vector<std::string> RequestIn::getEnvVector( void ) {
    return this->envVector;
}

std::string RequestIn::getBoundary( void ) {
    return this->boundary;
}
