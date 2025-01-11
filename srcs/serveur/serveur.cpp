
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

	this->pfd.fd = this->socket_fd;
	this->pfd.events = POLLIN;
	this->pfd.revents = 0;

	std::cout << ORANGE "hello constructor from serveur\n" GREEN "server listen . . .\n" RESET << std::endl;
}

serveur::~serveur()
{
	close(this->socket_fd);
	std::cout << ORANGE "destructor serveur" RESET << std::endl;
}

int		serveur::getFD()
{
	return this->socket_fd;
}

void routine_servor()
{
	try
	{
		serveur server;
		// while (true)
		// {
			if (poll(&server.pfd, 1, -1) && server.pfd.revents == POLLIN)
			{
				client	*new_client = creat_client(server.getFD());
				if (!new_client)
				{
					std::cerr << RED "error creat whit new client" RESET << std::endl;
					// continue;
				}
				delete new_client;
			// }
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
