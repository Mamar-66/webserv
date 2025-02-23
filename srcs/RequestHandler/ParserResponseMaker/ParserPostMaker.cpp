#include "../../../includes/Parser.hpp"

std::vector<std::string> RequestIn::PushResponse( void /* ParseConfig& config */ ) {
    std::map<int, std::string> mapCode = initCodeMap();
    std::string rootedDir = "./html"; // CHANGE WITH CONF
    std::vector<std::string> vectorElems;

    vectorElems.push_back("HTTP/1.1 ");
    vectorElems.push_back(intToString(this->codeHTTP));
    vectorElems.push_back(" ");
    vectorElems.push_back(this->stringCode);
    vectorElems.push_back("\r\n");

    if (this->codeHTTP - 400 < 0) {
        MyTriple<std::string, std::string, bool> fileToSave = makeFilePost(*this);

        createFile(fileToSave);

        if (fileToSave.third == false)
            this->codeHTTP = 201;
        else
            this->codeHTTP = 204;
        this->stringCode = mapCode[this->codeHTTP];
        

        std::string htmlResponse;
        if (this->codeHTTP == 201) {
            std::string catFile = rootedDir + this->uri + "index.html";
            htmlResponse = loadPage(catFile);
        }
        else
            htmlResponse = "";
        vectorElems.push_back("Content-Type: text/html\r\n");
        vectorElems.push_back("Content-Length: ");
        vectorElems.push_back(intToString(static_cast<int>(htmlResponse.size()) - 1));
        vectorElems.push_back("\r\n");
        vectorElems.push_back("Connexion: ");
        vectorElems.push_back("close\r\n");
        vectorElems.push_back("Date: ");
        vectorElems.push_back(getHttpDate());
        vectorElems.push_back("\r\n\r\n");
        vectorElems.push_back(htmlResponse); // SOME ADJUSTEMENT TO GIVE AND NEED TO DO FUNCTION TO INTERPRET POST
    }
    else {
        MyVector<int> vectorCodeGenerate;
        vectorCodeGenerate.push_back(404);
        vectorCodeGenerate.push_back(400);
        vectorCodeGenerate.push_back(500);// change to get with config
        std::map<int, std::string> mapCodeHtml;
        mapCodeHtml[404] = "./html/errors_400/error_404.html";
        mapCodeHtml[400] = "./html/errors_400/error_400.html";
        mapCodeHtml[500] = "./html/errors_500/error_500.html";

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
    return vectorElems;
}
