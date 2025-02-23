#include "../../../includes/Parser.hpp"

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

std::string checkRedir(std::string& ch) { //, ParseConfig& conf) {
    const MyStack<std::string> stackRedir = create1Stack(ch);
    

    /* Creation of artificial redirection, need to change with config.getRedir() */
    std::map<std::string, std::string> mapRedir;

    mapRedir["/d"] = "./";
    mapRedir["/e"] = "/conect/index.html";
    mapRedir["/d/d"] = "/exemple2-1";
    mapRedir["/d/e"] = "/exemple2-2";
    mapRedir["/e/d"] = "/exemple2-3";
    mapRedir["/e/e"] = "/exemple2-4";

    /* End */
    std::map<MyStack<std::string>, std::string> mapCorresp = createmapStackRedir(mapRedir);

    for (std::map<MyStack<std::string>, std::string>::iterator it = mapCorresp.begin(); it != mapCorresp.end(); it++)
        if (it->first == stackRedir)
            return it->second;
    return ch;
}

std::string concatenateVectors(std::vector<std::string> vec) {
    std::string toReturn = "";

    for (std::vector<std::string>::iterator it=vec.begin(); it != vec.end(); it++) {
        toReturn += *it;
    }

    return toReturn;


}

std::string loadPage(std::string& catFile) {
    std::string htmlResponse = "";
    std::ifstream file(catFile.c_str());
    std::string line;
    while (std::getline(file, line)) {
        htmlResponse += line;
        htmlResponse += "\n";
        //htmlResponse += "\r\n";
    }
    return htmlResponse;
}

std::vector<std::string> listDirectory(std::string path) {
    std::vector<std::string> vector;
    DIR* dir = opendir(path.c_str());  // Ouvre le répertoire

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {  // Parcourt chaque entrée
        vector.push_back(entry->d_name);  // Affiche le nom du fichier/dossier
    }

    closedir(dir);  // Ferme le répertoire
    return vector;
}

std::vector<std::string> makeBodyIndex( RequestIn& req ) {
    std::vector<std::string> vector;
    std::string rootedDir = "./html"; /* Change with config.getRootDir() */
    std::vector<std::string> listDir = listDirectory(rootedDir + req.getURI());

    vector.push_back("<!DOCTYPE html>\n<html>\n<head>\n<title>Index of ");
    vector.push_back(req.getURI());
    vector.push_back("/</title>\n    <style>\n        body { font-family: Arial, sans-serif; }\n        table { width: 100%; border-collapse: collapse; }\n        th, td { padding: 8px; border-bottom: 1px solid #ddd; }\n    </style>\n</head>\n<body>\n    <h1>Index of ");
    vector.push_back(req.getURI());
    vector.push_back("/</h1>\n    <table>\n        <tr><th>Name</th></tr>");
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

    std::string bodyAutoIndex = concatenateVectors(makeBodyIndex(req));

    vector.push_back("HTTP/1.1 200 OK\r\n");
    vector.push_back("Content-Type: text/html; charset=UTF-8\r\n");
    vector.push_back("Content-Length: ");
    vector.push_back(intToString(static_cast<int>(bodyAutoIndex.size()) - 1));
    vector.push_back("\r\nConnection: close\r\nDate: ");
    vector.push_back(getHttpDate());
    vector.push_back("\r\n\r\n");
    vector.push_back(bodyAutoIndex);

    return vector;
}

void addElemToStr(std::string& str, MyStack<std::string>& stack) {
    std::string elem = stack.popTop();
    str += "/";
    str += elem;
    std::cout << stack.empty() << std::endl;
}

std::vector<std::string> StrToVector(std::string str) {
    std::istringstream iss(str);
    std::string line;
    std::vector<std::string> toReturn;


    while (std::getline( iss, line )) {
        line += "\n";
        toReturn.push_back(line);
    }

    return toReturn;
}
