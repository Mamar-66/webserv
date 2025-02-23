#include "../../../includes/Parser.hpp"

std::string postGetBoundary(std::string& str) {
	std::string delim = "boundary=";
	std::string subs = str.substr(str.find(delim) + delim.size());
	std::cout << subs << std::endl;
	return subs;
}

std::string getLittleInBig(std::string& input, std::string& delim, std::string& deli2) {
	size_t beg = input.find(delim) + delim.size();
	size_t end = input.find(deli2, beg);

	std::string subs = input.substr(beg, end - beg);
	return subs;
}

void createFile(MyTriple<std::string, std::string, bool>& triple) {

	std::string file = "./uploadServer/" + triple.first;
	std::ofstream monFichier(file.c_str(), std::ios::binary);
	if (monFichier) {
		monFichier << triple.second;
		monFichier.close();
	}
}
MyTriple<std::string, std::string, bool>  makeFilePost(RequestIn& input) {
	std::string delimName = "filename=\"";
	std::string deli2Name = "\"";
	std::string delimFile = "\r\n\r\n";
	std::string deli2File = "--" + input.getBoundary();
	MyTriple<std::string, std::string, bool> triple;


	triple.first = getLittleInBig(input.getBody(), delimName, deli2Name);
	triple.second = getLittleInBig(input.getBody(), delimFile, deli2File);

	std::string fileEx = "./uploadServer/" + triple.first;
	if (access(fileEx.c_str(), F_OK) == 0)
		triple.third = true;
	else
		triple.third = false;
	std::cout << ORANGE << triple.second << RESET << std::endl;
	return triple;
}
