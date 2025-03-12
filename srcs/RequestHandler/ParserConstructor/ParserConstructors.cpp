#include "../../../includes/Webserv.h"
#include <iostream>
#include <unistd.h>

/* Copy Constructor */
RequestIn::RequestIn(RequestIn& cpy) :  monitor(cpy.monitor), cl(cpy.cl) {
    this->mapParse = cpy.mapParse;
    this->mapCount = cpy.mapCount;
}

/* Assignement operator */
RequestIn& RequestIn::operator=(RequestIn& cpy) {
    this->mapParse = cpy.mapParse;
    this->mapCount = cpy.mapCount;
    return *this;
}

/* Default Constructor */
RequestIn::RequestIn(client& request, monitoring& monitor) : monitor(monitor), cl(request) {
    this->locSet = false;
    std::istringstream stream(request.getInput());
    this->serv = monitor.servors[request.getFDFather()];
    std::string line;
    std::string key, value;

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
            std::cerr << "A1" << std::endl;
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
    std::istreambuf_iterator<char> eos;
    std::string s(std::istreambuf_iterator<char>(stream), eos);
    this->body = s;
}

RequestIn::~RequestIn() {}
