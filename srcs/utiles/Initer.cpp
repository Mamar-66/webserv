#include "../../includes/StaticClasses/Initer.hpp"

Initer::Initer() {}

Initer::Initer(Initer& cpy) {
    (void)cpy;
}

Initer& Initer::operator=(Initer& cpy) {
    (void)cpy;
    return *this;
}

Initer::~Initer() {}


std::map<int, std::string> Initer::initCodeMap( void ) {
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

std::string Initer::loadPage(std::string& catFile) {
    std::string htmlResponse = "";
    std::ifstream file(catFile.c_str());
    if (!file.is_open())
    	return "";
    std::string line;
    while (std::getline(file, line)) {
        htmlResponse += line;
        htmlResponse += "\n";
        //htmlResponse += "\r\n";
    }
    return htmlResponse;
}

std::string Initer::generateSessionId() {
    std::string str = "";
    std::string base = "0123456789abcdef";

    return "TO DO";
    // for (int i = 0; i < 4; ++i) {
    //     for (int j = 0; j < 8; ++j) {
            
    //     }
    // }

}

std::string Initer::makeTheSample(std::string code, std::string str, std::string sample) {
	std::string page = Initer::loadPage(sample);
    if (page.empty()) {
        return page;
    }
	std::string fromCode = "000";
	std::string fromStr = "XXX";
	std::size_t pos = 0;
	
	while ((pos = page.find(fromCode, pos)) != std::string::npos) {
		page.replace(pos, fromCode.length(), code);
		pos += code.length();
	}
	pos = 0;
	while ((pos = page.find(fromStr, pos)) != std::string::npos) {
		page.replace(pos, fromStr.length(), str);
		pos += str.length();
	}
	return page;
}

void	Initer::creat_servor(std::vector<std::string> &cut_str_serv, monitoring &moni)
{
	int size = cut_str_serv.size();
	int i = 0;

	if (size <= 0)
		throw std::runtime_error("Error no compliant server");

	serveur 				*servor;

	for (std::vector<std::string>::iterator it = cut_str_serv.begin(); it != cut_str_serv.end(); ++it)
	{
		if (i >= size)
			break;

		servor = Initer::malloc_serv(*it);
		if (servor != NULL)
		{
			for (std::vector<pollfd>::iterator it = servor->all_pollfd.begin(); it != servor->all_pollfd.end(); ++it)
			{
				moni.servors[it->fd] = servor;
				moni.all_all_pollfd.push_back(*it);
				moni.all_pollfd_servor.push_back(*it);
			}
		}
	}
}

serveur *Initer::malloc_serv(std::string &str)
{
	serveur *servor = NULL;

	std::cout << str << std::endl;

	try
	{
		servor = new serveur(str);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return NULL;
	}

	return servor;
}
