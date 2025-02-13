#include "Parser.hpp"

std::string getHttpDate() {
    char buffer[100];
    std::time_t now = std::time(NULL);
    std::tm* gmt = std::gmtime(&now);
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);
    return std::string(buffer);
}
std::string intToString(int value) {
    std::ostringstream oss;
    oss << value; // Écrit l'entier dans le flux
    return oss.str(); // Retourne la chaîne
}

bool isDirectory(const std::string& path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) == 0) {
        // Vérifie si le type correspond à un répertoire
        return (pathStat.st_mode & S_IFMT) == S_IFDIR;
    }
    return false; // Chemin invalide ou erreur
}

bool isFile(const std::string& path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) == 0) {
        // Vérifie si le type correspond à un fichier régulier
        return (pathStat.st_mode & S_IFMT) == S_IFREG;
    }
    return false; // Chemin invalide ou erreur
}


std::map<int, std::string> initCodeMap() {
    std::map<int, std::string> mapCode;

    mapCode[100] = "Continue";
    mapCode[101] = "Switching Protocols";
    mapCode[102] = "Processing";
    mapCode[103] = "Early Hints";

    mapCode[200] = "OK";
    mapCode[201] = "Created";
    mapCode[202] = "Accepted";
    mapCode[203] = "Non-Authoritative Information";
    mapCode[204] = "No Content";
    mapCode[205] = "Reset Content";
    mapCode[206] = "Partial Content";
    mapCode[207] = "Multi-Status";
    mapCode[208] = "Already Reported";
    mapCode[210] = "Content Different";
    mapCode[226] = "IM Used";

    mapCode[300] = "Multiple Choices";
    mapCode[301] = "Moved Permanently";
    mapCode[302] = "Found";
    mapCode[303] = "See Other";
    mapCode[304] = "Not Modified";
    mapCode[305] = "Use Proxy";
    mapCode[307] = "Temporary Redirect";
    mapCode[308] = "Permanent Redirect";
    mapCode[310] = "Too many Redirects";

    mapCode[400] = "Bad Request";
    mapCode[401] = "Unauthorized";
    mapCode[402] = "Payment Required";
    mapCode[403] = "Forbidden";
    mapCode[404] = "Not Found";
    mapCode[405] = "Method Not Allowed";
    mapCode[406] = "Not Acceptable";
    mapCode[407] = "Proxy Authentication Required";
    mapCode[408] = "Request Time-out";
    mapCode[409] = "Conflict";
    mapCode[410] = "Gone";
    mapCode[411] = "Length Required";
    mapCode[412] = "Precondition Failed";
    mapCode[413] = "Request Entity Too Large";
    mapCode[414] = "Request-URI Too Long";
    mapCode[415] = "Unsupported Media Type";
    mapCode[416] = "Requested range unsatisfiable";
    mapCode[417] = "Expectation Failed";
    mapCode[418] = "I'm a teapot";
    mapCode[419] = "Page expired";
    mapCode[421] = "Bad mapping / Misdirected Request";
    mapCode[422] = "Unprocessable entity";
    mapCode[423] = "Locked";
    mapCode[424] = "Method Failure";
    mapCode[425] = "Too early";
    mapCode[426] = "Upgrade Required";
    mapCode[427] = "Invalid digital signature";
    mapCode[428] = "Precondition Required";
    mapCode[429] = "Too many request";
    mapCode[431] = "Request Header Fields Too Large";
    mapCode[449] = "Retry With";
    mapCode[450] = "Blocked by Windows Parental Controls";
    mapCode[451] = "Unavailable For Legal Reasons";
    mapCode[456] = "Unrecoverable Error";

    mapCode[500] = "Internal Server Error";
    mapCode[501] = "Not Implemented";
    mapCode[502] = "Bad Gateway ";
    mapCode[503] = "Service anavailable";
    mapCode[504] = "Gateway Timeout";
    mapCode[505] = "HTTP Version not supported";
    mapCode[506] = "Variant Also negotiates";
    mapCode[507] = "Insufficient storage";
    mapCode[508] = "Loop detected";
    mapCode[509] = "Bandwidth Limit Exceeded";
    mapCode[510] = "Not extended";
    mapCode[511] = "Network authentication required";

    return mapCode;
}

int compNb(std::string s) {
    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        if ((s[i] < '0' || s[i] > '9') && s[i] != '\r' && s[i] != '\n' && s[i] != ' ') {
            std::cout << "kkkkkkk: " << s[i] << std::endl;
            return 0;
        }
    }
    return (1);
}


int AnalyseStacks(std::stack<char> elems, std::stack<MyTriple<char, char, char> > comas)
{
    int count1 = 0;
    int count2 = 0;
    char c;
    MyTriple<char, char, char> triple;

    /* Check coherence {} [] */
    while (! elems.empty() ) {
        c = elems.top();
        elems.pop();
        if (c == '[')
            count1++;
        else if (c == '{')
            count2++;
        else if (c == '}')
            count2--;
        else if (c == ']')
            count1--;
            
        if (count1 > 0 || count2 > 0)
            return 400;
    }
    if (count1 != 0 || count2 != 0)
        return 400;


    /* Check coherence comas */
    while (! comas.empty()) {
        triple = comas.top();
        comas.pop();

        if (triple.first == '[' || triple.first == '{' || triple.third == ']' || triple.third == '}')
            return 400;
    }
    return 200;
}


char removeSpaces(std::string body, int cas, int i) {
    switch (cas) {
        case BEFORE:
            i--;
            while (body[i] == ' ')
                i--;
            return body[i];
        case AFTER:
            i++;
            while (body[i] == ' ')
                i++;
            return body[i];
        default:
            return ('?');
    }
}

int analyseSub(std::string body, int i) {
    
    if (removeSpaces(body, BEFORE, i) != '"')
        return (0);
    i++;
    while (body[i] == ' ') 
        i++;

    if (body[i] == '"')
        return (1);
    else {
        while (body[i] != ' ' && body[i] ) {
            if (body[i] < '0' || body[i] > '9')
                return (0);
            i++;
        }
        return (1);
    }
}

int parseCatFile(std::string f) { //, ParseConfig& conf) {
    std::istringstream iss(f);
    std::istringstream is2("./"); // Change to conf.getRoot()

    MyStack<std::string> stackRepo;
    MyStack<std::string> stackDefault;

    std::string s;
    while ( std::getline( iss, s, '/' ) ) {
        if (s == ".." && stackRepo.empty())
            return 0; 
        if (s == "..") 
            stackRepo.pop();
        else if (s == "." && stackRepo.empty())
            stackRepo.push(s);
        else if (s == ".") 
            ;
        else 
            stackRepo.push(s);
    }

    while ( std::getline( is2, s, '/' ) ) {
        if (s == ".." && stackDefault.empty())
            return 0; 
        else if (s == "..")
            stackDefault.pop();
        else if (s == "." && stackDefault.empty())
            stackDefault.push(s);
        else if (s == ".") 
            ;
        else 
            stackDefault.push(s);
    }
    stackRepo.reverse();
    stackDefault.reverse();
    while (!(stackDefault.empty())) {
        if (stackDefault.popTop() != stackRepo.popTop())
            return 0;
    }
    return 1;
}

int parseJson(std::string& body) {
    std::stack<char> stackElem;
    std::stack<MyTriple<char, char, char> > stackComa;
    std::stack<MyTriple<char, char, std::string> > stackKeyValue;

    if (body[0] != '[' && body[0] != '{')
        return 400;
    stackElem.push(body[0]);
    for (int i = 1; i < static_cast<int>(body.size()) - 1; i++) {
        if (body[i] == '"') {
            i++;
            while (body[i] != '"' && body[i] != '\0')
                i++;
            if (body[i] != '"')
                return 400;
            i++;
        }
        if (body[i] == '[' || body[i] == '{')
            stackElem.push(body[0]);

        if (body[i] == ',')
            stackComa.push(MyTriple<char, char, char>(removeSpaces(body, BEFORE, i), body[i], removeSpaces(body, AFTER, i)));
        if (body[i] == ':' && !(analyseSub(body, i)))
            return 400;
    }
    stackElem.push(body[body.size() - 1]);

    return AnalyseStacks(stackElem, stackComa);
}

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
    mapRedir["/e"] = "/exemple1-2";
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

    //std::cout << "-----------------------------\n" << toReturn << "\n-----------------------------" << std::endl;
    return toReturn;


}

bool isTypeOk(MyVector<std::string> typesServer, std::string& typeAccepted) {
    MyVector<std::string> types;
    std::istringstream iss(typeAccepted);
    std::string type;

    while (std::getline(iss, type, ',')) {
        type.erase(std::remove(type.begin(), type.end(), ' '));
        types.push_back(type);
    }

    for (std::vector<std::string>::iterator it = types.begin(); it != types.end(); it++) {
        if (typesServer == *it) 
            return true;
    }
    return false;
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
std::string checkCGI(std::string& str) {
    MyStack<std::string> stackPath = create1Stack(str);
    std::string stringCGI = "";
    addElemToStr(stringCGI, stackPath);
    while ( ( isFile(stringCGI) == false || access(stringCGI.c_str(), X_OK) != 0 ) && stackPath.empty() == false) {
        addElemToStr(stringCGI, stackPath);
        std::cout << stackPath.empty() << std::endl;
    }
    if (isFile(stringCGI) == true && access(stringCGI.c_str(), X_OK) == 0)
        return stringCGI;
    return "";
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == delimiter) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += str[i];
        }
    }
    
    // Ajouter le dernier token s'il n'est pas vide
    if (!token.empty()) {
        tokens.push_back(token);
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        size_t pos = tokens[i].find(';');
        if (pos != std::string::npos) {
            tokens[i] = tokens[i].substr(0, pos);
        }
    }

    return tokens;
}
