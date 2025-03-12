#include "../../includes/Webserv.h"

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

std::string RequestIn::getBoundary( void ) {
    return this->boundary;
}

monitoring& RequestIn::getMonitor( void ) {
    return this->monitor;
}

Location& RequestIn::getLoc( void ) {
    return this->loc;
}

serveur* RequestIn::getServ( void ) {
    return this->serv;
}

std::string& RequestIn::getsessionId( void ) {
    return this->sessionId;
}

bool    &RequestIn::getLocSet( void ) {
    return this->locSet;
}

void    RequestIn::setCode( int code ) {
    this->codeHTTP = code;
}

void    RequestIn::setUri( std::string uri) {
    this->uri = uri;
}

void    RequestIn::setLoc( Location& loc ) {
    this->loc = loc;
}
