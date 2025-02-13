#include "../../includes/Parser.hpp"

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
