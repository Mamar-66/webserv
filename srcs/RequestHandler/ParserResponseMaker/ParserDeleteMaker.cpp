/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserDeleteMaker.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbarbe <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:28:00 by sbarbe            #+#    #+#             */
/*   Updated: 2025/02/13 11:28:16 by sbarbe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

std::vector<std::string> RequestIn::DeleteResponse()
{
    std::map<int, std::string> mapCode = Initer::initCodeMap();
    std::vector<std::string> response;

    std::string tempUri = this->uri;
    
    if (tempUri == "/")
        tempUri = "";
    else
        tempUri = "." + tempUri;
    std::cout << "Tentative de suppression : " << tempUri << std::endl;

    if (!Checker::isFile(tempUri) && !Checker::isDirectory(tempUri))
        this->codeHTTP = 404; // Not Found
    else
    {
        if (Checker::isFile(tempUri))
        {
            if (std::remove(tempUri.c_str()) == 0)
                this->codeHTTP = 204; // No Content (succès)
            else
                this->codeHTTP = 403; // Forbidden (échec de suppression)
        } 
        else if (Checker::isDirectory(tempUri))
        {
            if (std::system(("rm -r " + tempUri).c_str()) == 0)
                this->codeHTTP = 204; // Succès
            else
                this->codeHTTP = 500; // Erreur serveur
        }
    }

    this->stringCode = mapCode[this->codeHTTP];

    response.push_back("HTTP/1.1 " + Conversion::intToString(this->codeHTTP) + " " + this->stringCode + "\r\n");
    response.push_back("Connection: close\r\n");
    response.push_back("Date: " + GenericGetter::getHttpDate() + "\r\n\r\n");

    return response;
}
