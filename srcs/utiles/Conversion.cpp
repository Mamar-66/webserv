#include "../../includes/StaticClasses/Conversion.hpp"

Conversion::Conversion() {}
Conversion::~Conversion() {}
Conversion::Conversion(Conversion& cpy) {
    (void)cpy;
}
Conversion& Conversion::operator=(Conversion& cpy) {
    (void)cpy;
    return *this;
}


int	Conversion::stringToInt(std::string& str) {
    int result;
    std::istringstream stream(str);
    
    stream >> result;

    if (stream.fail() || !stream.eof()) {
        throw(std::runtime_error("Error, invalid chunk size"));
    }

    return result;
}

std::string Conversion::intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string Conversion::vectorToString(std::vector<std::string> vec) {
    std::string toReturn = "";

    for (std::vector<std::string>::iterator it=vec.begin(); it != vec.end(); it++) {
        toReturn += *it;
    }

    return toReturn;
}

std::vector<std::string> Conversion::StrToVector(std::string str) {
    std::istringstream iss(str);
    std::string line;
    std::vector<std::string> toReturn;


    while (std::getline( iss, line )) {
        line += "\n";
        toReturn.push_back(line);
    }

    return toReturn;
}
