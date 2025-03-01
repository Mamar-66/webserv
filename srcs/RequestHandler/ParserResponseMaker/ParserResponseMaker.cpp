#include "../../../includes/Parser.hpp"
#include "../../../includes/serveur.hpp"

std::vector<std::string> RequestIn::GetResponse( void /* ParseConfig& config */ ) {
    std::cout << RED << this->cl.getInput() << RESET << std::endl;
    std::map<int, std::string> mapCode = Initer::initCodeMap();

    std::string rootedDir = GenericGetter::findRoot(*this); /* Change with config.getRootDir() */

    std::vector<std::string> vectorElems;

    std::string temp = checkRedir(*this);
    if (temp != this->uri && temp != "")
        this->codeHTTP = 302;
    if (temp != "")
        this->uri = temp; // rEVOTIR

    std::string catFile = rootedDir + this->uri;
    // std::cout << catFile << std::endl;
    bool AutoIndexToRemove = this->loc.getAutoindex();
    if (this->loc.verifauto == false)
        AutoIndexToRemove = this->serv->getAutoindex();

    if (!(Checker::parseCatFile(catFile, *this)))
        this->codeHTTP = 403;
    if (this->codeHTTP == 200 || this->codeHTTP / 100 == 3) {
        if (Checker::isDirectory(catFile)) {
            if (catFile[catFile.size() - 1] != '/')
                catFile += "/";
            std::string defaultP = this->loc.getIndex();
            if (defaultP.empty())
                defaultP = this->serv->getIndex();
            if (defaultP.empty())
                defaultP = "index.html";
            catFile = catFile + defaultP; /* Change to config.getDefaultPage */
            if (!(Checker::isFile(catFile))) {
                if (AutoIndexToRemove == false)
                    this->codeHTTP = 403;
                else
                    return makeAutoIndex(*this);
            }

        }
    }

    this->stringCode = mapCode[this->codeHTTP];
    
    vectorElems.push_back("HTTP/1.1 ");
    vectorElems.push_back(Conversion::intToString(this->codeHTTP));
    vectorElems.push_back(" ");
    vectorElems.push_back(this->stringCode);
    vectorElems.push_back("\r\n");

    std::map<int, std::string> mapCodeHtml = this->serv->getErrorPage();
    std::map<int, std::string> mapCodeLocation = this->loc.getErrorPage();
    for (std::map<int, std::string>::iterator it = mapCodeLocation.begin(); it != mapCodeLocation.end(); it++)
        mapCodeHtml[it->first] = it->second;
    MyVector<int> vectorCodeGenerate;

    for (std::map<int, std::string>::iterator it = mapCodeHtml.begin(); it != mapCodeHtml.end(); ++it) {
        vectorCodeGenerate.push_back(it->first);
    }

    if (this->codeHTTP - 400 >= 0) {
		
		std::string htmlResponse;
        if (vectorCodeGenerate == this->codeHTTP)
            htmlResponse = Initer::loadPage(mapCodeHtml[this->codeHTTP]);
        else
            htmlResponse = Initer::makeTheSample(Conversion::intToString(this->codeHTTP), this->stringCode, "./html/conect/errors/errors_sample/error_sample.html");

    	std::cout << "Code: " << this->codeHTTP << std::endl;

        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
        if (this->mapParse.find("Cookie") != this->mapParse.end() )
            std::cout << "truc: |" <<  this->mapParse["Cookie"].substr(1) << std::endl;
        if (this->mapParse.find("Cookie") == this->mapParse.end() || this->monitor.mapCookie.find(this->mapParse["Cookie"].substr(1)) == this->monitor.mapCookie.end()) {
            std::string str = "00000000-00000000-00000000-00000000";
            std::string str2 = Initer::generateSessionId();
            Cookie user(str, str2);
            this->monitor.mapCookie["SessionID=" + user.getSessionID()] = user;
            std::cout << "SessionID=" + user.getSessionID() << " : " << this->monitor.mapCookie[user.getSessionID()].getUserID() <<std::endl;
            this->mapParse["Cookie"] = user.getSessionID();
            vectorElems.push_back("Set-Cookie: SessionID=");
            vectorElems.push_back(this->mapParse["Cookie"]);
            vectorElems.push_back("\r\n");
            std::cout << this->monitor.mapCookie[this->mapParse["Cookie"]].getSessionID() << " :: " <<  this->monitor.mapCookie[this->mapParse["Cookie"]].getUserID() <<std::endl;
        }
        vectorElems.push_back("Connection: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(GenericGetter::getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
    }

    if (this->codeHTTP / 100 == 3 || this->codeHTTP / 100 == 2) {
//        std::cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT: " << catFile << " " << (catFile.find_last_of('?')) << " " << catFile.substr(0, catFile.find_last_of('?')) << " " <<  (int)Checker::isFile(catFile.substr(0, catFile.find_last_of('?'))) << " " << catFile.substr(0, catFile.find_last_of('?')).substr(catFile.find_last_of('.')) << " " << (this->loc.getCgiExt() == catFile.substr(0, catFile.find_last_of('?')).substr(catFile.find_last_of('.'))) << " " << (access(catFile.substr(0, catFile.find_last_of('?')).c_str(), X_OK) == 0) << std::endl;
std::cerr << "HHHHHHHHHHHH" << catFile << std::endl;
if (Checker::isFile(catFile.substr(0, catFile.find_last_of('?'))) && this->loc.getCgiExt() == catFile.substr(0, catFile.find_last_of('?')).substr(catFile.substr(0, catFile.find_last_of('?')).find_last_of('.')) && access(catFile.substr(0, catFile.find_last_of('?')).c_str(), X_OK) == 0)
{
	std::cout << "???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????" << std::endl;
	return this->holdCGI(mapCodeHtml);
}
		std::cout << "Code: " << this->codeHTTP << std::endl;
        std::string htmlResponse = Initer::loadPage(catFile);
        vectorElems.push_back("Content-Type: ");
        vectorElems.push_back(this->mimeAccept);
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
        if (this->mapParse.find("Cookie") != this->mapParse.end())
            std::cout << "truccccccc: |" <<  this->mapParse["Cookie"].substr(1) << std::endl;
        if (this->mapParse.find("Cookie") == this->mapParse.end() || this->monitor.mapCookie.find(this->mapParse["Cookie"].substr(1)) == this->monitor.mapCookie.end()) {
            std::string str = "00000000-00000000-00000000-00000000";
            std::string str2 = Initer::generateSessionId();
            Cookie user(str, str2);
            this->monitor.mapCookie["SessionID=" + user.getSessionID()] = user;
            std::cout << "SessionID=" + user.getSessionID() << " : " << this->monitor.mapCookie[user.getSessionID()].getUserID() <<std::endl;
            this->mapParse["Cookie"] = user.getSessionID();
            vectorElems.push_back("Set-Cookie: SessionID=");
            vectorElems.push_back(this->mapParse["Cookie"]);
            vectorElems.push_back("\r\n");
            std::cout << this->monitor.mapCookie[this->mapParse["Cookie"]].getSessionID() << " :: " <<  this->monitor.mapCookie[this->mapParse["Cookie"]].getUserID() <<std::endl;
        }
        vectorElems.push_back("Connection: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(GenericGetter::getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
	}
    return vectorElems;
}

// std::vector<std::string> RequestIn::PushResponse( void /* ParseConfig& config */ ) {
//     std::map<int, std::string> mapCode = Initer::initCodeMap();
//     std::string rootedDir = "./"; /* Change with config.getRootDir() */
//     std::vector<std::string> vectorElems;
//     std::string temp = checkRedir(this->uri);
//     if (temp != this->uri)
//         this->codeHTTP = 302;
//     this->uri = temp;

//     std::string catFile = rootedDir + this->uri;

//     bool AutoIndexToRemove = false;

//     if (!(Checker::parseCatFile(catFile)))
//         this->codeHTTP = 403;
//     if (this->codeHTTP == 200 || this->codeHTTP / 100 == 3) {
//         this->codeHTTP = 201;
//         if (Checker::isDirectory(catFile)) {
//             catFile = catFile + "index.html"; /* Change to config.getDefaultPage */
//             if (!(Checker::isFile(catFile))) {
//                 if ( AutoIndexToRemove == false ) // config.getAutoIndex() == false)
//                     this->codeHTTP = 403;
//                 else
//                     return makeAutoIndex();
//             }
//             else if (access(catFile.c_str(), X_OK) != 0)
//                 this->codeHTTP = 405;
//         }
//         else if (!(Checker::isFile(catFile)))
//             this->codeHTTP = 404;
//     }

//     this->stringCode = mapCode[this->codeHTTP];
    
//     vectorElems.push_back("HTTP/1.1 ");
//     vectorElems.push_back(Conversion::intToString(this->codeHTTP));
//     vectorElems.push_back(" ");
//     vectorElems.push_back(this->stringCode);
//     vectorElems.push_back("\r\n");

//     std::string htmlResponse = getAdressesError( this->codeHTTP /*, config*/);

//     vectorElems.push_back("Content-Type: text/html\r\n");
//     vectorElems.push_back("Content-Length: ");
//     vectorElems.push_back(Conversion::intToString(static_cast<int>(htmlResponse.size()) - 1));
//     vectorElems.push_back("\r\n");
//     vectorElems.push_back("Connexion: ");
//     vectorElems.push_back("close\r\n");
//     vectorElems.push_back("Date: ");
//     vectorElems.push_back(GenericGetter::getHttpDate());
//     vectorElems.push_back("\r\n\r\n");
//     vectorElems.push_back(htmlResponse); // SOME ADJUSTEMENT TO GIVE AND NEED TO DO FUNCTION TO INTERPRET POST
//     return vectorElems;
// }


std::string RequestIn::makeResponse( void ) {
    std::vector<std::string> vectorElems;

	if (this->method == "POST") {
		vectorElems = this->PushResponse();
	}
	else if (this->method == "DELETE") {
        vectorElems = this->DeleteResponse();
    }
    else {
        vectorElems = this->GetResponse();
    }
    return Conversion::vectorToString(vectorElems);
}

//CHECK TIMEOUT
