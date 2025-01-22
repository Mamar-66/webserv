
#include "../../includes/serveur.hpp"

/* -------------------------------------------------------- */
/* ----------------------CONSTRUCTOR----------------------- */
/* -------------------------------------------------------- */

void serveur::creatSocket()
{
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::ostringstream oss;

	if (this->socket_fd == -1)
	{
		oss << errno;
		throw std::runtime_error(RED "Error create socket server\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}

	memset(&this->server_addr, 0, sizeof(this->server_addr));
	this->server_addr.sin_family = AF_INET;
	this->server_addr.sin_addr.s_addr = INADDR_ANY;
	this->server_addr.sin_port = htons(this->port);
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

void serveur::addConfig(const std::string &strConfig)
{
	std::string p = return_word_after("listen", strConfig);
	int 		number = 0;	
	if (p.empty())
		throw std::runtime_error(RED "Error config file for chearch listen");

	try
	{
		number = stringToInt(p);
	}
	catch(const std::exception& e)
	{
		throw;
	}
	
	this->port = number;

	std::cout << strConfig << std::endl;
	std::cout << this->port << std::endl;
}

serveur::serveur(const std::string &strConfig)
{
	try
	{
		this->addConfig(strConfig);
		this->creatSocket();
		this->bindSocket();
		this->stratListening();
	}
	catch (const std::exception &e)
	{
		throw;
	}

	std::cout << ORANGE "hello constructor from serveur\n" GREEN "server listen . . .\n" RESET << std::endl;
}

/* -------------------------------------------------------- */
/* -------------------------------------------------------- */
/* -------------------------------------------------------- */

serveur::~serveur()
{
	close(this->socket_fd);
	std::cout << ORANGE "destructor serveur" RESET << std::endl;
}

int		serveur::getFD()
{
	return this->socket_fd;
}

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
