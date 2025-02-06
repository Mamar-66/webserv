
#include "../../includes/serveur.hpp"
#include "Location.hpp"

/* -------------------------------------------------------- */
/* --------------- CONSTRUCTOR / DESTRUCTOR --------------- */
/* -------------------------------------------------------- */

void serveur::creatSocket(config &myconfig, std::map<std::string, Location> &location)
{
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::ostringstream oss;

	if (this->socket_fd == -1)
	{
		oss << errno;
		throw std::runtime_error(RED "Error create socket server\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}
	(void)location;
	memset(&this->server_addr, 0, sizeof(this->server_addr));
	this->server_addr.sin_family = AF_INET;
	this->server_addr.sin_addr.s_addr = INADDR_ANY;
	this->server_addr.sin_port = htons(myconfig.port);
}

void serveur::bindSocket()
{
	std::ostringstream oss;

	if (bind(this->socket_fd, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) < 0)
	{
		oss << errno;
		throw std::runtime_error(RED "Error bind servor\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}
}

void serveur::stratListening()
{
	std::ostringstream oss;

	if (listen(this->socket_fd, MAX_CLIENTS) < 0)
	{
		oss << errno;
		throw std::runtime_error(RED "Error listen\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}

	this->pfd.fd = this->socket_fd;
	this->pfd.events = POLLIN;
	this->pfd.revents = 0;
}

void serveur::addConfig(const std::string &strConfig, config &myconfig, std::map<std::string, Location> &location)
{
	(void)location;
	std::string p = return_word_after("listen", strConfig);
	if (p.empty())
		throw std::runtime_error(RED "Error config file for chearch listen");

	try
	{
		myconfig.port = stringToInt(p);
		myconfig.config_name = return_word_after("server_name", strConfig);
		//this->host = return_word_after("host", strConfig);
	}
	catch(const std::exception& e)
	{
		throw;
	}
}

serveur::serveur(const std::string &strConfig)
{
	try
	{
		config myconfig;
		std::string current;
		std::map<std::string, Location> location;
		std::vector<std::string> lines = splitLines(strConfig);
		location = parseLocations(lines, current);
   		for (size_t i = 0; i < lines.size(); ++i)
		{
			lines[i] = normalizeSpaces(lines[i]);
			if (myconfig.op == true && location[current].op == true)
					parsconfigL(lines[i], location, current);
			else
					parsconfig(myconfig, lines[i], location, current);
		}
		if (location[current].op == true || myconfig.op == true)
				throw std::runtime_error("Error : missing '{' or '}'");
		if (myconfig.config_name.empty() || myconfig.port == 0)
			throw std::runtime_error("Error : it is mandatory to have the Server Port and Name, server");
		this->addConfig(strConfig, myconfig, location);
		this->creatSocket(myconfig, location);
		this->bindSocket();
		this->stratListening();
	}
	catch (const std::exception &e)
	{
		throw;
	}

	std::cout << ORANGE "hello constructor from serveur" RESET << std::endl;
}

serveur::~serveur()
{
	close(this->socket_fd);
	// delete[] this->pfd;
	// delete this;
	std::cout << ORANGE "destructor serveur" RESET << std::endl;
}

/* -------------------------------------------------------- */
/* ----------------------- GETTER ------------------------- */
/* -------------------------------------------------------- */

const int	&serveur::getFD()
{
	return this->socket_fd;
}

/* -------------------------------------------------- */

std::string serveur::return_word_after(const std::string &word, const std::string &str)
{
	size_t pos = str.find(word);

	if (pos == std::string::npos)
		return NULL;
	
	pos += word.length();

	while (pos < str.length() && (str[pos] == ' ' || str[pos] == '\t'))
		++pos;
	
	size_t endPos = str.find(';', pos);
	if (endPos == std::string::npos)
		return NULL;
	
	return str.substr(pos, endPos - pos);
}

int serveur::stringToInt(const std::string &str)
{
	std::istringstream iss(str);
	int number;

	if (!(iss >> number))
		throw std::invalid_argument("Invalid input: Not a number");

	return number;
}
