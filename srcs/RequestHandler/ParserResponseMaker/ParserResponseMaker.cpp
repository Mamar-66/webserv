#include "../../../includes/Parser.hpp"

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

std::string RequestIn::makeResponse(const std::string& input) {
    std::vector<std::string> vectorElems;
    if (this->method == "POST") {
        vectorElems = this->PostResponse(input);
    }

    else if (this->method == "DELETE") {
        vectorElems = this->DeleteResponse();
    }

    
    else {
        vectorElems = this->GetResponse();
    }

    for (std::vector<std::string>::iterator it=vectorElems.begin(); it < vectorElems.end(); it++) {
        std::cout << *it;
    }
    std::cout << "------------------------------" << std::endl;
    return concatenateVectors(vectorElems);
}
