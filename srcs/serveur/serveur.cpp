
#include "../../includes/Webserv.h"

monitoring::monitoring()
{

}

monitoring::~monitoring()
{
	if (this->clients.size() > 0)
	{
		for (std::map<int, client*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
			delete it->second;
	}
	if (this->servors.size() > 0)
	{
		std::set<serveur*> free_servor;

		for (std::map<int, serveur*>::iterator it = this->servors.begin(); it != this->servors.end(); ++it)
				free_servor.insert(it->second);
		for(std::set<serveur*>::iterator it = free_servor.begin(); it != free_servor.end(); ++it)
			delete *it;
	}
	this->clients.clear();
	this->servors.clear();
}

/* -------------------------------------------------------- */
/* --------------- CONSTRUCTOR / DESTRUCTOR --------------- */
/* -------------------------------------------------------- */

serveur::serveur(serveur& cpy) {
	(void)cpy;
}
serveur& serveur::operator=(serveur& cpy) {
	(void)cpy;
	return *this;
}

int	serveur::creatSocket(const int &port)
{
	int return_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in	server_addr;
	std::ostringstream oss;

	if (return_socket == -1)
	{
		oss << errno;
		throw std::runtime_error(RED "Error create socket server\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}

	int opt = 1;
	setsockopt(return_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(return_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		close(return_socket);
		oss << errno;
		throw std::runtime_error(RED "Error bind servor\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}

	if (listen(return_socket, MAX_CLIENTS) < 0)
	{
		oss << errno;
		throw std::runtime_error(RED "Error listen\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}

	this->pfd.fd = return_socket;
	this->pfd.events = POLLIN;
	this->pfd.revents = 0;

	// std::cerr << ORANGE "hello constructor from serveur " << this->config_name << " FD = " << return_socket << RESET << std::endl;

	return return_socket;
}

void serveur::addConfig(const std::string &strConfig)
{
		std::string current;
		std::vector<std::string> lines = splitLines(strConfig);
		parseLocations(lines, current);
		std::vector<std::string> locationNames;
		
   		for (size_t i = 0; i < lines.size(); ++i)
		{
			lines[i] = normalizeSpaces(lines[i]);
			if (this->op == true && location[current].op == true)
				parsconfigL(lines[i], location, current);
			else
				parsconfig(lines[i], location, current);
		}
		if (location[current].op == true || this->op == true)
				throw std::runtime_error("Error : missing '{' or '}'");
		if (this->config_name.empty() || this->port.empty())
			throw std::runtime_error("Error : it is mandatory to have the Server Port, server");
}

int	serveur::listen_port(const int &port)
{
	int	return_fd = -1;

	try
	{
		return_fd = this->creatSocket(port);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return return_fd;
	}
	
	return return_fd;
}

void serveur::bind_port()
{
	int 	size = this->port.size();
	int 	tmp_fd;

	for (int i = 0; i < size; ++i)
	{
		tmp_fd = this->listen_port(this->port[i]);
		if (tmp_fd > 0)
		{
			this->servor_socket.push_back(tmp_fd);
			this->all_pollfd.push_back(this->pfd);
		}
	}

	if (this->servor_socket.empty())
		throw std::runtime_error("Error servor no creat");
}

serveur::serveur(const std::string &strConfig)
{
	try
	{
		this->addConfig(strConfig);
		this->bind_port();
	}
	catch (const std::exception &e)
	{
		throw;
	}

	// std::cerr << ORANGE "hello constructor from serveur FD = " <<  RESET << std::endl;
}

serveur::~serveur()
{
	// std::cerr << ORANGE "destructor serveur" << RESET << std::endl;
	int size = this->servor_socket.size();
	for (int i = 0; i < size; i++)
		close(this->servor_socket[i]);
	this->servor_socket.clear();
}

/* -------------------------------------------------------- */
/* ----------------------- GETTER ------------------------- */
/* -------------------------------------------------------- */


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

int	monitoring::where_are_fd_pipe(const int &fd)
{
	std::map<int, client*>::iterator it = this->clients.begin();

	if (fd > 0)
	{
		for (; it != this->clients.end(); ++it)
		{
			if (it->second->getStatusCgi())
			{
				if (fd == it->second->pipe_write[1])
					return it->first;
				else if (fd == it->second->pipe_read[0])
					return it->first;
			}
		}
	}
	return -1;
}
