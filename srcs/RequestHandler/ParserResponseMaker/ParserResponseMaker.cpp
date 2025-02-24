#include "../../../includes/Parser.hpp"
#include "../../../includes/serveur.hpp"

std::vector<std::string> RequestIn::GetResponse( void /* ParseConfig& config */ ) {
    std::map<int, std::string> mapCode = initCodeMap();

    std::string rootedDir = "./html"; /* Change with config.getRootDir() */

    std::vector<std::string> vectorElems;

    std::string temp = checkRedir(this->uri);
    if (temp != this->uri)
        this->codeHTTP = 302;
    
    this->uri = temp;

    std::string catFile = rootedDir + this->uri;
    // std::cout << catFile << std::endl;
    bool AutoIndexToRemove = true;

    if (!(parseCatFile(catFile)))
        this->codeHTTP = 403;
    if (this->codeHTTP == 200 || this->codeHTTP / 100 == 3) {
        if (isDirectory(catFile)) {
            if (catFile[catFile.size() - 1] != '/')
                catFile += "/";
            catFile = catFile + "index.html"; /* Change to config.getDefaultPage */
            if (!(isFile(catFile))) {
                if (AutoIndexToRemove == false)
                    this->codeHTTP = 403;
                else
                    return makeAutoIndex(*this);
            }

        }
        else if (!(isFile(catFile))) {
            std::string stringCGI = checkCGI(catFile);

            if (stringCGI == "")
                this->codeHTTP = 404;
            else
                return StrToVector(this->holdCGI());
        }
    }

    this->stringCode = mapCode[this->codeHTTP];
    
    vectorElems.push_back("HTTP/1.1 ");
    vectorElems.push_back(intToString(this->codeHTTP));
    vectorElems.push_back(" ");
    vectorElems.push_back(this->stringCode);
    vectorElems.push_back("\r\n");

    MyVector<int> vectorCodeGenerate;
    vectorCodeGenerate.push_back(404);
    vectorCodeGenerate.push_back(400);
    vectorCodeGenerate.push_back(500);// change to get with config

    std::map<int, std::string> mapCodeHtml;
    mapCodeHtml[404] = "./html/conect/errors/errors_400/error_404.html";
    mapCodeHtml[400] = "./html/conect/errors/errors_400/error_400.html";
    mapCodeHtml[500] = "./html/conect/errors/errors_500/error_500.html";

    if (this->codeHTTP - 400 >= 0) {
        std::string htmlResponse;
        if (vectorCodeGenerate == this->codeHTTP)
            htmlResponse = loadPage(mapCodeHtml[this->codeHTTP]);
        else
            htmlResponse = makeTheSample(intToString(this->codeHTTP), this->stringCode, "./html/conect/errors/errors_sample/error_sample.html");

    	std::cout << "Code: " << this->codeHTTP << "\nhtmlResponse: "<< htmlResponse << std::endl;

        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
        std::cout << this->mapParse["Cookie"] << std::endl;
        if (this->mapParse.find("Cookie") == this->mapParse.end() || this->monitor.mapCookie.find(this->mapParse["Cookie"]) == this->monitor.mapCookie.end()) {
            std::string str = "00000000-00000000-00000000-00000000";
            std::string str2 = generateSessionId();
            Cookie user(str, str2);
            this->monitor.mapCookie["SessionID=" + user.getSessionID()] = user;
            std::cout << "SessionID=" + user.getSessionID() << " : " << this->monitor.mapCookie[user.getSessionID()].getUserID() <<std::endl;
            this->mapParse["Cookie"] = user.getSessionID();
            vectorElems.push_back("Set-Cookie: SessionID=");
            vectorElems.push_back(this->mapParse["Cookie"]);
            vectorElems.push_back("\r\n");
            std::cout << this->monitor.mapCookie[this->mapParse["Cookie"]].getSessionID() << " :: " <<  this->monitor.mapCookie[this->mapParse["Cookie"]].getUserID() <<std::endl;
        }
        else {
            vectorElems.push_back("Cookie: ");
            vectorElems.push_back(this->mapParse["Cookie"]);
            vectorElems.push_back("\r\n");
        }
        vectorElems.push_back("Connection: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
    }

    if (this->codeHTTP / 100 == 3 || this->codeHTTP / 100 == 2) {
        std::string htmlResponse = loadPage(catFile);
        vectorElems.push_back("Content-Type: ");
        vectorElems.push_back(this->mimeAccept);
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
        if (this->mapParse.find("Cookie") == this->mapParse.end() || this->monitor.mapCookie.find(this->mapParse["Cookie"]) == this->monitor.mapCookie.end()) {
            std::string str = "00000000-00000000-00000000-00000000";
            std::string str2 = "NoneID";
            Cookie user(str, str2);
            this->monitor.mapCookie["SessionID=" + user.getSessionID()] = user;
            std::cout << "SessionID=" + user.getSessionID() << " : " << this->monitor.mapCookie[user.getSessionID()].getUserID() <<std::endl;
            this->mapParse["Cookie"] = user.getSessionID();
            vectorElems.push_back("Set-Cookie: SessionID=");
            vectorElems.push_back(this->mapParse["Cookie"]);
            vectorElems.push_back("\r\n");
            std::cout << this->monitor.mapCookie[this->mapParse["Cookie"]].getSessionID() << " :: " <<  this->monitor.mapCookie[this->mapParse["Cookie"]].getUserID() <<std::endl;
        }
        else {
            vectorElems.push_back("Cookie: ");
            vectorElems.push_back(this->mapParse["Cookie"]);
            vectorElems.push_back("\r\n");
        }
        vectorElems.push_back("Cookie: ");
        vectorElems.push_back(this->mapParse["Cookie"]);
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Connection: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
    }
    return vectorElems;
}

// std::vector<std::string> RequestIn::PushResponse( void /* ParseConfig& config */ ) {
//     std::map<int, std::string> mapCode = initCodeMap();
//     std::string rootedDir = "./"; /* Change with config.getRootDir() */
//     std::vector<std::string> vectorElems;
//     std::string temp = checkRedir(this->uri);
//     if (temp != this->uri)
//         this->codeHTTP = 302;
//     this->uri = temp;

//     std::string catFile = rootedDir + this->uri;

//     bool AutoIndexToRemove = false;

//     if (!(parseCatFile(catFile)))
//         this->codeHTTP = 403;
//     if (this->codeHTTP == 200 || this->codeHTTP / 100 == 3) {
//         this->codeHTTP = 201;
//         if (isDirectory(catFile)) {
//             catFile = catFile + "index.html"; /* Change to config.getDefaultPage */
//             if (!(isFile(catFile))) {
//                 if ( AutoIndexToRemove == false ) // config.getAutoIndex() == false)
//                     this->codeHTTP = 403;
//                 else
//                     return makeAutoIndex();
//             }
//             else if (access(catFile.c_str(), X_OK) != 0)
//                 this->codeHTTP = 405;
//         }
//         else if (!(isFile(catFile)))
//             this->codeHTTP = 404;
//     }

//     this->stringCode = mapCode[this->codeHTTP];
    
//     vectorElems.push_back("HTTP/1.1 ");
//     vectorElems.push_back(intToString(this->codeHTTP));
//     vectorElems.push_back(" ");
//     vectorElems.push_back(this->stringCode);
//     vectorElems.push_back("\r\n");

//     std::string htmlResponse = getAdressesError( this->codeHTTP /*, config*/);

//     vectorElems.push_back("Content-Type: text/html\r\n");
//     vectorElems.push_back("Content-Length: ");
//     vectorElems.push_back(intToString(static_cast<int>(htmlResponse.size()) - 1));
//     vectorElems.push_back("\r\n");
//     vectorElems.push_back("Connexion: ");
//     vectorElems.push_back("close\r\n");
//     vectorElems.push_back("Date: ");
//     vectorElems.push_back(getHttpDate());
//     vectorElems.push_back("\r\n\r\n");
//     vectorElems.push_back(htmlResponse); // SOME ADJUSTEMENT TO GIVE AND NEED TO DO FUNCTION TO INTERPRET POST
//     return vectorElems;
// }

void 	RequestIn::init_envp(client &cl)
{
	std::string tmp;

	tmp = "URI=" + getURI();
	cl.envp[0] = strdup(tmp.c_str());
	tmp = "METHOD=" + getMethod();
	cl.envp[1] = strdup(tmp.c_str());
	tmp = "BODY=" + getBody();
	cl.envp[2] = strdup(tmp.c_str());
	cl.envp[3] = NULL;
}

std::string RequestIn::makeResponse(monitoring &moni, client &cl) {
    std::vector<std::string> vectorElems;

	if (getURI().find(".py") != std::string::npos)
	{
		init_envp(cl);
		start_CGI(moni, cl);
		return "cgi";
	}
	else if (this->method == "POST") {
		vectorElems = this->PushResponse();
	}
	else if (this->method == "DELETE") {
        vectorElems = this->DeleteResponse();
    }
    else {
        vectorElems = this->GetResponse();
    }

    // for (std::vector<std::string>::iterator it=vectorElems.begin(); it < vectorElems.end(); it++) {
    //     std::cout << *it;
    // }
    // std::cout << "------------------------------" << std::endl;
    return concatenateVectors(vectorElems);
}
