
#include "../../includes/serveur.hpp"
#include "Location.hpp"

/* -------------------------------------------------------- */
/* ----------------------CONSTRUCTOR----------------------- */
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

	this->pfd = new struct pollfd[MAX_CLIENTS + 1];

	for (int i = 0; i <= MAX_CLIENTS; i++)
	{
		this->pfd[i].fd = -1;
		this->pfd[i].events = 0;
		this->pfd[i].revents = 0;
	}

	this->pfd[0].fd = this->socket_fd;
	this->pfd[0].events = POLLIN;
	this->pfd[0].revents = 0;
}

// void serveur::addConfig(const std::string &strConfig)
// {
// 	std::string p = return_word_after("listen", strConfig);
// 	if (p.empty())
// 		throw std::runtime_error(RED "Error config file for chearch listen");

// 	try
// 	{
// 		this->port = stringToInt(p);
// 		this->config_name = return_word_after("server_name", strConfig);
// 		this->host = return_word_after("host", strConfig);
// 		// p = return_word_after("client_max_body_size", strConfig);
// 		// this->client_max_body_size = stringToInt(p);
// 	}
// 	catch(const std::exception& e)
// 	{
// 		throw;
// 	}

// 	// std::cout << strConfig << std::endl;
// 	// std::cout << this->port << std::endl;
// }

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
		// this->addConfig(strConfig);
		this->creatSocket(myconfig, location);
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
	delete[] this->pfd;
	// delete this;
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
