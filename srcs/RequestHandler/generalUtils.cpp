#include "../../includes/Webserv.h"

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

std::string removeUriFile(std::string uri, RequestIn& req) {
    std::string test = GenericGetter::findRoot(req) + uri;
    if (req.getMethod() == "DELETE") {
        if (!(Checker::isDirectory(req.getURI()))) {
            return (uri.substr(0, uri.find_last_of('?')).substr(0, uri.find_last_of('/') + 1));
        }
    }
    if (Checker::isFile(test.substr(0, test.find_last_of('?'))) == true)
    {
        return uri.substr(0, uri.find_last_of('?')).substr(0, uri.find_last_of('/') + 1);
    }
    return uri;
}

int countWords(const std::string& str) {
    std::istringstream stream(str);
    std::string word;
    int wordCount = 0;

    while (stream >> word) {
        wordCount++;
    }
    
    return wordCount;
}

std::vector<std::string> splitt(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));

    return tokens;
}

int count_alloc_env(RequestIn& req, std::map<int, std::string> mapErrorHtml)
{
    int i = 2;
    if (!req.getURI().empty())
        i++;
    if (!req.getMethod().empty())
    {
        if (req.getMethod().compare(0, 4, "GET") == 0)
            i += 2;
        else
            i++;
    }
    i += mapErrorHtml.size();
    return (i);
}

char* my_strdup(const std::string& str) {
    char* copy = new char[str.size() + 1];
    std::strcpy(copy, str.c_str());
    return copy;
}

char **creatEnv(RequestIn& req, std::map<int, std::string> mapErrorHtml)
{
    int i = 0;
    size_t pos = req.getURI().find_last_of('?');
    std::string before = (pos != std::string::npos) ? req.getURI().substr(0, pos) : req.getURI();
    std::string after = (pos != std::string::npos) ? req.getURI().substr(pos + 1) : "";
    char **env = new char*[count_alloc_env(req, mapErrorHtml)];
    std::string root = GenericGetter::findRoot(req);
    env[i++] = my_strdup(("EXEC=" + root + before).c_str());
    env[i++] = my_strdup(("URI=" + req.getURI()).c_str());
    if (!after.empty())
        env[i++] = my_strdup(("FORM=" + after).c_str());
    env[i++] = my_strdup(("METHOD=" + req.getMethod()).c_str());
    for (std::map<int, std::string>::const_iterator it = mapErrorHtml.begin(); it != mapErrorHtml.end(); ++it)
    {
        std::string error_entry = "ERROR_PAGE_" + Conversion::intToString(it->first) + "=" + it->second;
        env[i++] = my_strdup(error_entry.c_str());
    }
    env[i] = NULL;
    return (env);
}

