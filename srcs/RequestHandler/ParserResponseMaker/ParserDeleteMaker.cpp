/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserDeleteMaker.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmichel- <rmichel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:28:00 by sbarbe            #+#    #+#             */
/*   Updated: 2025/03/12 15:57:05 by rmichel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Webserv.h"
#include "../../../includes/serveur.hpp"

std::vector<std::string> RequestIn::DeleteResponse()
{
    std::cerr << this->cl.getInput() << std::endl;
    std::map<int, std::string> mapCode = Initer::initCodeMap();
    std::vector<std::string> vectorElems;

    std::string tempUri = this->uri;
    std::string username = this->monitor.mapCookie [this->sessionId].getUsername();
    // std::cerr << username << std::endl;

    std::cerr << "Tentative de suppression : " << tempUri << std::endl;

    std::string add = "";
    if (GenericGetter::findRoot(*this)[GenericGetter::findRoot(*this).size() - 1] != '/')
        add = "/";
    std::cerr << ORANGE << "./uploadServer/" + this->monitor.mapCookie[this->sessionId].getRepertory() + tempUri.substr(tempUri.find_last_of('/')) << RESET << std::endl;
    if (this->codeHTTP / 100 == 2)
    {
        if (access(("./uploadServer/" + this->monitor.mapCookie[this->sessionId].getRepertory() + tempUri.substr(tempUri.find_last_of('/'))).c_str(), F_OK) != 0 && !Checker::isDirectory(("./uploadServer/" + this->monitor.mapCookie[this->sessionId].getRepertory() + tempUri.substr(tempUri.find_last_of('/')))))
            this->codeHTTP = 404;
        else
        {
            if (std::system(("rm -r ./uploadServer/" + this->monitor.mapCookie[this->sessionId].getRepertory() + tempUri.substr(tempUri.find_last_of('/'))).c_str()) == 0)
                this->codeHTTP = 204;
            else
                this->codeHTTP = 403;
        }
    }

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
            std::cerr << ORANGE << mapCodeHtml[this->codeHTTP] << RESET << std::endl;
            htmlResponse = Initer::loadPage(mapCodeHtml[this->codeHTTP]);
        }
        else
            htmlResponse = Initer::makeTheSample(Conversion::intToString(this->codeHTTP), this->stringCode, "./html/conect/errors/errors_sample/error_sample.html");

    	// std::cerr << "Code: " << this->codeHTTP << std::endl;

        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size())));// - (!(htmlResponse.empty()) - 1)));
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
        std::string htmlResponse = "";
        vectorElems.push_back("Content-Type: ");
        vectorElems.push_back(this->mimeAccept);
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size())));// - (!(htmlResponse.empty()) - 1)));
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

// std::string removeUriFile(std::string uri, RequestIn& req) {
//     std::string test = GenericGetter::findRoot(req) + uri;
//     if (req.getMethod() == "DELETE") {
//         if (!(Checker::isDirectory(req.getURI()))) {
//             return (uri.substr(0, uri.find_last_of('?')).substr(0, uri.find_last_of('/') + 1));
//         }
//     }
//     //std::cerr << uri.substr(0, uri.find_last_of('?')) << Checker::isFile(uri.substr(0, uri.find_last_of('?'))) << std::endl;
//     if (Checker::isFile(test.substr(0, test.find_last_of('?'))) == true)
//     {
//         // std::cerr << uri.substr(0, uri.find_last_of('?')).substr(0, uri.find_last_of('/') + 1) << std::endl;
//         return uri.substr(0, uri.find_last_of('?')).substr(0, uri.find_last_of('/') + 1);
//     }
//     return uri;
// }
