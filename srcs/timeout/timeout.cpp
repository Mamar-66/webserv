#include "../../includes/Webserv.h"

std::string timeout(client& cl, monitoring& monitor, std::string code, std::map<int, std::string> map) {
    std::string response;
    std::map<int, std::string> mapCode = Initer::initCodeMap();
    std::string body;
    if (map.find(Conversion::stringToInt(code)) != map.end())
    {
        body = Initer::loadPage(monitor.servors[cl.getFDFather()]->getErrorPage()[Conversion::stringToInt(code)]);
    }
    else
    {
        body = Initer::makeTheSample(code, mapCode[Conversion::stringToInt(code)], "./html/conect/errors/errors_sample/error_sample.html");
    }
    response = "HTTP/1.1 ";
    response += code;
    response += " ";
    response += mapCode[Conversion::stringToInt(code)];
    response += "\r\nContent-Type: text/html\r\nContent-Length: ";
    response += Conversion::intToString(static_cast<int>(body.size()) - (body.size() != 0));
    response += "\r\nConnection: close\r\nDate: ";
    response += GenericGetter::getHttpDate();
    response += "\r\n\r\n";
    response += body;
    return response;
}
