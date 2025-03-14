#include "../../../includes/Webserv.h"

MyStack<std::string> create1Stack(const std::string& ch) {
    std::istringstream iss(ch);
    std::string s;
    MyStack<std::string> toReturn;

    while ( std::getline(iss, s, '/') ) 
        toReturn.push(s);

    return toReturn;
}

std::map<MyStack<std::string>, std::string> createmapStackRedir(std::map<std::string, std::string>& map) {
    std::map<MyStack<std::string>, std::string> toReturn;

    for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
        toReturn[create1Stack(it->first)] = it->second;

    return toReturn;
}

std::string checkRedir(RequestIn& req, int n) {

    if (n >= 15) {
        req.setCode(310);
        return "";
    }

    std::string retour = req.getLoc().getRetur();
    if (retour == "" || req.getServ()->location.find(retour) == req.getServ()->location.end() || req.getServ()->location[retour].getRetur().empty())
    {
        return retour;
    }
    req.setUri(retour);
    req.setLoc(req.getServ()->location[removeUriFile(req.getURI(), req)]);
    return checkRedir(req, n+1);
}

std::vector<std::string> listDirectory(std::string path) {
    std::vector<std::string> vector;
    DIR* dir = opendir(path.c_str());

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        vector.push_back(entry->d_name);
    }

    closedir(dir);
    return vector;
}

std::vector<std::string> makeBodyIndex( RequestIn& req ) {
    std::vector<std::string> vector;
    std::string rootedDir = GenericGetter::findRoot(req);
    std::vector<std::string> listDir = listDirectory(rootedDir + req.getURI());
    std::string add = "";

    if (req.getURI()[req.getURI().size() - 1] != '/')
        add = "/";

    vector.push_back("<!DOCTYPE html>\n<html>\n<head>\n<title>Index of ");
    vector.push_back(req.getURI() + add);
    vector.push_back("</title>\n    <style>\n        body { font-family: Arial, sans-serif; }\n        table { width: 100%; border-collapse: collapse; }\n        th, td { padding: 8px; border-bottom: 1px solid #ddd; }\n    </style>\n</head>\n<body>\n    <h1>Index of ");
    vector.push_back(req.getURI() + add);
    vector.push_back("</h1>\n    <table>\n        <tr><th>Name</th></tr>");
    for (int i = 0; i < static_cast<int>(listDir.size()); i++) {
        vector.push_back("<tr><td><a href=\"");
        vector.push_back(listDir[i]);
        vector.push_back("\">");
        vector.push_back(listDir[i]);
        vector.push_back("</a></td></tr>");
    }
    vector.push_back("</table>\n</body>\n</html>");
    return vector;
}

std::vector<std::string> makeAutoIndex( RequestIn& req ) {
    std::vector<std::string> vector;

    std::string bodyAutoIndex = Conversion::vectorToString(makeBodyIndex(req));

    vector.push_back("HTTP/1.1 200 OK\r\n");
    vector.push_back("Content-Type: text/html; charset=UTF-8\r\n");
    vector.push_back("Content-Length: ");
    vector.push_back(Conversion::intToString(static_cast<int>(bodyAutoIndex.size())));
    vector.push_back("\r\nConnection: close\r\nDate: ");
    vector.push_back(GenericGetter::getHttpDate());
    vector.push_back("\r\n\r\n");
    vector.push_back(bodyAutoIndex);

    return vector;
}

void addElemToStr(std::string& str, MyStack<std::string>& stack) {
    std::string elem = stack.popTop();
    str += "/";
    str += elem;
}
