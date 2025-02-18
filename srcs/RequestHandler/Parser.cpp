#include "Parser.hpp"
#include <iostream>
#include <unistd.h>

RequestIn::RequestIn(RequestIn& cpy) {
    this->mapParse = cpy.mapParse;
    this->mapCount = cpy.mapCount;
}

RequestIn& RequestIn::operator=(RequestIn& cpy) {
    this->mapParse = cpy.mapParse;
    this->mapCount = cpy.mapCount;
    return *this;
}

RequestIn::RequestIn(const std::string& request/* , char **envp */) {
    clientRequest = 100; //client value
    std::istringstream stream(request);
    std::string line;
    std::string key, value;
    std::vector<std::string> vecEnv;
    std::string chEnv;

    // for (int i = 0; envp[i] != NULL; i++) {
    //     chEnv = envp[i];
    //     vecEnv.push_back(chEnv);
    // }
    // vecEnv.push_back(chEnv);

    this->envVector = vecEnv;
    this->codeHTTP = 200;
    std::getline(stream, line);
    std::istringstream streamLine(line);
    std::string token;

    while (streamLine >> token)
        this->vector.push_back(token);

    std::getline(stream, line);
    if (line[line.size() - 1] != ('\r')) {
        this->codeHTTP = 400;
        return ;
    }
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    while (! line.empty()) {
        std::cout << "line: " << line << std::endl;
        streamLine.str(line);
        streamLine.clear();

        std::getline(streamLine, key, ':');
        if (key.find('\n') != std::string::npos) {
            this->codeHTTP = 400;
            return ;
        }
        std::getline(streamLine, value);
        std::cout << key << " - " << value << std::endl;
        this->mapParse[key] = value;
        if (this->mapCount.find(key) != this->mapCount.end())
            this->mapCount[key] += 1;
        else 
            this->mapCount[key] = 1;
        std::getline(stream, line, '\n');
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    }
    // std::cout << "Map :\n--------------------------- Headers -----------------------------------\n";
    // for (std::map<std::string, std::string>::const_iterator it = this->mapParse.begin(); it != this->mapParse.end(); ++it) 
    //     std::cout << it->first << " : " << it->second << std::endl;

    // std::cout << "\n--------------------------------- Body -----------------------------\n";
    // std::getline(stream, this->body, '\0');
    // std::cout << this->body << std::endl;
    // std::cout << "\n--------------------------------- End -----------------------------\n";
    this->length = 0;
    //sleep(2);
}

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


std::vector<std::string> RequestIn::GetResponse( void /* ParseConfig& config */ ) {
    std::map<int, std::string> mapCode = initCodeMap();

    std::string rootedDir = "./html"; /* Change with config.getRootDir() */

    std::vector<std::string> vectorElems;

    std::string temp = checkRedir(this->uri);
    if (temp != this->uri)
        this->codeHTTP = 302;
    
    this->uri = temp;

    std::string catFile = rootedDir + this->uri;
    std::cout << catFile << std::endl;
    bool AutoIndexToRemove = true;


    if (!(parseCatFile(catFile)))
        this->codeHTTP = 403;
    if (this->codeHTTP == 200 || this->codeHTTP / 100 == 3) {
        if (isDirectory(catFile)) {
            catFile = catFile + "/index.html"; /* Change to config.getDefaultPage */
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
    mapCodeHtml[404] = "./html/errors_400/error_404.html";
    mapCodeHtml[400] = "./html/errors_400/error_400.html";
    mapCodeHtml[500] = "./html/errors_500/error_500.html";

    if (this->codeHTTP - 400 >= 0) {
        std::string htmlResponse;
        if (vectorCodeGenerate == this->codeHTTP)
            htmlResponse = loadPage(mapCodeHtml[this->codeHTTP]);
        else
            htmlResponse = "";


        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
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
        vectorElems.push_back("Connection: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse);
    }
    return vectorElems;
}

std::string RequestIn::getURI( void ) {
    return this->uri;
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

// std::string RequestIn::makeResponse(const std::string& input) {
//     std::vector<std::string> vectorElems;
//     if (this->method == "POST") {
//         vectorElems = this->PostResponse(input);
//     }

//     else if (this->method == "DELETE") {
//         vectorElems = this->DeleteResponse();
//     }

    
//     else {
//         vectorElems = this->GetResponse();
//     }

//    // for (std::vector<std::string>::iterator it=vectorElems.begin(); it < vectorElems.end(); it++) {
//    //     std::cout << *it;
//    // }
//     std::cout << "------------------------------" << std::endl;
//     return concatenateVectors(vectorElems);
// }

RequestIn::~RequestIn() {}

std::map<std::string, std::string> RequestIn::getMap( void ) {
    return this->mapParse;
}

int RequestIn::getCode( void ) {
    return this->codeHTTP;
}

std::string RequestIn::holdCGI( void ) {
    int pipefd[2];
    
    if ( pipe(pipefd)  == -1 )  {
        this->codeHTTP = 500;
        return "";
    }

    pid_t pid = fork();

    if (pid < 0) {
        this->codeHTTP = 500;
        return "";
    }
    else if (pid == 0) {
        return "";/*
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        std::string serverName = "SERVER_NAME=" + "myWebserv.com"; // Change "myWebserv.com" to a #DEFINE variable
        std::string serverProtocol = "HTTP/1.1";
        std::string serverPort = static_cast<std::string>(this->port); //change to parserClass
        std::string scriptName = "kkk"

        this->envVector.push_back();*/
    }
    else 
        return "";
    
}
