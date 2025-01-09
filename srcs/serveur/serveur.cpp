
#include "../../includes/serveur.hpp"

serveur::serveur()
{
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socket_fd == -1)
	{
		std::ostringstream oss;
		oss << errno;
		throw std::runtime_error(RED "Error create socket server\nCode error : " + oss.str() + "\nError code value : " + std::string(strerror(errno)));
	}
	std::cout << ORANGE "hello constructor from serveur\nfd socker servor = " << this->socket_fd << RESET <<  std::endl;
}

serveur::~serveur()
{
	close(this->socket_fd);
	std::cout << ORANGE  "destructor serveur" RESET << std::endl;
}

void	routine_servor()
{	
	try 
	{
		serveur server;
	}
	catch (const std::exception &e)
	{
		throw;
	}
}
