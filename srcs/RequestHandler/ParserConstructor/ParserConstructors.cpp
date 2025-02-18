#include "../../../includes/Parser.hpp"
#include <iostream>
#include <unistd.h>

/* Copy Constructor */
RequestIn::RequestIn(RequestIn& cpy) {
    this->mapParse = cpy.mapParse;
    this->mapCount = cpy.mapCount;
}

/* Assigne;ent operator */
RequestIn& RequestIn::operator=(RequestIn& cpy) {
    this->mapParse = cpy.mapParse;
    this->mapCount = cpy.mapCount;
    return *this;
}

/* Default Constructor */
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
    vecEnv.push_back(chEnv);

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
        streamLine.str(line);
        streamLine.clear();

        std::getline(streamLine, key, ':');
        if (key.find('\n') != std::string::npos) {
            this->codeHTTP = 400;
            return ;
        }
        std::getline(streamLine, value);
        this->mapParse[key] = value;
        if (this->mapCount.find(key) != this->mapCount.end())
            this->mapCount[key] += 1;
        else 
            this->mapCount[key] = 1;
        std::getline(stream, line, '\n');
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    }
    this->length = 0;
}

RequestIn::~RequestIn() {}
