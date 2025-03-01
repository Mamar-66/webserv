#include "../../includes/StaticClasses/GenericGetter.hpp"

std::string GenericGetter::getHttpDate() {
    char buffer[100];
    std::time_t now = std::time(NULL);
    std::tm* gmt = std::gmtime(&now);
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);
    return std::string(buffer);
}

std::string GenericGetter::postGetBoundary(std::string& str) {
	std::string delim = "boundary=";
	std::string subs = str.substr(str.find(delim) + delim.size());
	return subs;
}

std::string GenericGetter::getLittleInBig(std::string& input, std::string& delim, std::string& deli2) {
	size_t beg = input.find(delim) + delim.size();
	size_t end = input.find(deli2, beg);

	std::string subs = input.substr(beg, end - beg);
	return subs;
}

std::string GenericGetter::findRoot(RequestIn& req) {
    std::string root = req.getLoc().getRoot();
    if (root.empty()) 
        root = req.getServ()->getRoot();
    if (root.empty())
        root = "./html";
    return root;
}

char GenericGetter::removeSpaces(std::string body, int cas, int i) {
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
