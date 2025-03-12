#include "../../includes/Webserv.h"
#include <iostream>
#include <string>

Modifier::Modifier() {
}
Modifier::Modifier(Modifier& cpy) {
    (void)cpy;
}
Modifier& Modifier::operator=(Modifier& cpy) {
    (void)cpy;
    return *this;
}
Modifier::~Modifier() {
}

void Modifier::modifyMapCookies(monitoring& monitor, std::string& response) {
   // std::cerr << response.substr(response.find("HTTP/1.1 ") + 9, 3) << std::endl;
    if (response.substr(response.find("HTTP/1.1 ") + 9, 3) == "200") {
        // std::cerr << ORANGE << "GEN COOKIE" << RESET << std::endl;
        Cookie cookieTemp = Initer::initCookie(response);
        monitor.mapCookie[cookieTemp.getSessionID()] = cookieTemp;
    }
}
