#include "../../../includes/Webserv.h"

void createFile(MyTriple<std::string, std::string, bool>& triple, RequestIn& req) {

	(void)req;
	std::string file = "./uploadServer/"+ req.getMonitor().mapCookie[req.getsessionId()].getRepertory() + "/" + triple.first; // + req.getMonitor().mapCookie[req.getsessionId()].getRepertory() +
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
	if (deli2File == "--") {
		
	}
	MyTriple<std::string, std::string, bool> triple;

	triple.first = GenericGetter::getLittleInBig(input.getBody(), delimName, deli2Name);
	triple.second = GenericGetter::getLittleInBig(input.getBody(), delimFile, deli2File);

	std::string fileEx = "./uploadServer/" + input.getMonitor().mapCookie[input.getsessionId()].getRepertory() + "/" + triple.first;// FICHIER D'UPLOAD A DEF ??
	std::cerr << input.getsessionId() << ":" << fileEx << std::endl;
	if (access(fileEx.c_str(), F_OK) == 0)
		triple.third = true;
	else
		triple.third = false;
	// std::cerr << ORANGE << triple.second << RESET << std::endl;
	return triple;
}
