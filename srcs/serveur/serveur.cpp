
#include "../../includes/serveur.hpp"

serveur::serveur()
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
	this->server_addr.sin_port = htons(PORT);

	if (bind(this->socket_fd, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) < 0)
	{
		oss << errno;
		throw std::runtime_error(RED "Error bind servor\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}

	if (listen(this->socket_fd, MAX_CLIENTS) < 0)
	{
		oss << errno;
		throw std::runtime_error(RED "Error listen\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}
	std::cout << ORANGE "hello constructor from serveur\n" GREEN "server listen . . .\n" RESET << std::endl;
}

serveur::~serveur()
{
	close(this->socket_fd);
	std::cout << ORANGE "destructor serveur" RESET << std::endl;
}

void routine_servor()
{
	try
	{
		serveur server;
		// while (true)
		// {
		// }
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
