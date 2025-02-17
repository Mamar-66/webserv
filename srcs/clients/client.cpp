/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/17 17:35:37 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"
#include "../../includes/Parser.hpp"

/* -------------------------------------------------------- */
/* --------------CONSTRUCTOR / DESTRUCTOR------------------ */
/* -------------------------------------------------------- */

int		creat_socket_cgi()
{
	struct sockaddr_in 		serverAddr;
	struct sockaddr_in		addr;

	int 					return_socket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = 0;

	if (bind(return_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		throw std::runtime_error("error constructeur bind cgi");

	if (listen(return_socket, 5) == -1)
		throw std::runtime_error("error constructeur listen cgi");

	/* recup port choisi par le systeme */
	socklen_t addr_len = sizeof(addr);
	if (getsockname(return_socket, (struct sockaddr *)&addr, &addr_len) == -1)
		throw std::runtime_error("Error get port");
	unsigned short	port = ntohs(addr.sin_port);

	std::cout << "CGI Port : " << port << " fd : " << return_socket << std::endl;

	return return_socket;
}

client::client(int fdsocket, size_t size_body, bool listing) :
socket_fd(fdsocket), socket_fd_father(0), size_body(size_body), listing(listing), is_cgi(false)
{
	this->startTime = std::time(NULL);
	if (this->startTime == -1)
		throw std::runtime_error(RED "Failed to get the current time" RESET);

	this->clien_pollfd.fd = this->socket_fd;
	this->clien_pollfd.events = POLLIN | POLLOUT;
	this->clien_pollfd.revents = 0;

	int size = (int)sizeof(envp) / sizeof(envp[0]);
	for (int i = 0; i < size; ++i)
		this->envp[i] = NULL;

	std::cout << GREEN "creat client fd : " << this->socket_fd << "time = " << this->startTime << RESET << std::endl;
}

client::client(int fdsocket, int fdSocketFather, size_t size_body, bool listing) : 
socket_fd(fdsocket), socket_fd_father(fdSocketFather),
size_body(size_body), listing(listing), is_cgi(false)
{
	this->startTime = std::time(NULL);
	if (this->startTime == -1)
		throw std::runtime_error(RED "Failed to get the current time" RESET);

	this->clien_pollfd.fd = this->socket_fd;
	this->clien_pollfd.events = POLLIN | POLLOUT;
	this->clien_pollfd.revents = 0;

	int size = (int)sizeof(envp) / sizeof(envp[0]);
	for (int i = 0; i < size; ++i)
		this->envp[i] = NULL;

	std::cout << GREEN "creat client CGI fd : " << this->socket_fd << "time = " << this->startTime << RESET << std::endl;
}

client::~client()
{
	if (this->socket_fd != -1)
	{
		close(this->socket_fd);
		std::cout << RED "delete client whith close fd : " << this->socket_fd << RESET << std::endl;

		if (this->envp[0] != NULL)
		{
			int size = (int)sizeof(envp) / sizeof(envp[0]);
			for (int i = 0; i < size - 1; ++i)
				delete this->envp[i];
		}
		if (this->getStatusCgi())
		{
			close(this->pipe_write[0]);
			close(this->pipe_write[1]);
			close(this->pipe_read[0]);
			close(this->pipe_read[1]);
		}
	}
	else
		std::cout << RED "delete client warning don't close fd : " << this->socket_fd << RESET << std::endl;

}

/* -------------------------------------------------------- */
/* -------------------- GETTER ---------------------------- */
/* -------------------------------------------------------- */

const int	&client::getFD()
{
	return this->socket_fd;
}

const int &client::getFDFather()
{
	return this->socket_fd_father;
}

const std::time_t	&client::getStartTime()
{
	return this->startTime;
}

const std::string&	client::getInput(void)
{
	return this->input;
}

const std::string&	client::getOutput(void)
{
	return this->output;
}

const bool &client::getStatusCgi()
{
	return this->is_cgi;
}

const size_t	&client::getSizeBody()
{
	return this->size_body;
}

const bool	&client::getListing()
{
	return this->listing;
}

/* -------------------------------------------------------- */
/* -------------------- SETTER ---------------------------- */
/* -------------------------------------------------------- */

void	client::setInput(const std::string& str)
{
	this->input = str;
}

void	client::setOutput(const std::string& str)
{
	this->output = str;
}

void	client::setCgiTrue()
{
	this->is_cgi = true;
}
void client::setCgiFalse()
{
	this->is_cgi = false;
}

/* --------------------------------------------------- */

std::string url_decode(const std::string &url) {
    std::string decoded = "";
    for (size_t i = 0; i < url.length(); ++i) {
        if (url[i] == '%' && i + 2 < url.length()) {
            std::string hex = url.substr(i + 1, 2);
            char decoded_char = 0;
            if ((hex[0] >= '0' && hex[0] <= '9') || (hex[0] >= 'A' && hex[0] <= 'F') || (hex[0] >= 'a' && hex[0] <= 'f')) {
                if ((hex[1] >= '0' && hex[1] <= '9') || (hex[1] >= 'A' && hex[1] <= 'F') || (hex[1] >= 'a' && hex[1] <= 'f')) {
                    decoded_char = static_cast<char>(::strtol(hex.c_str(), NULL, 16));
                    decoded += decoded_char;
                    i += 2;
                    continue;
                }
            }
        }
        decoded += url[i];
    }
    return decoded;
}


std::string	read_request(const int &fd_client)
{
	std::string	return_str;
	char 		buff[2048];
	short		byte_read;

	int flags = fcntl(fd_client, F_GETFL, 0);
	if (flags == -1)
		std::runtime_error(RED "fcntl F_GETFL");
	if (fcntl(fd_client, F_SETFL, flags | O_NONBLOCK) == -1)
		std::runtime_error(RED "fcntl F_SETFL");

	do
	{
		std::memset(buff, 0, sizeof(buff));
		byte_read = read(fd_client, buff, sizeof(buff));

		if (byte_read > 0)
            return_str.append(buff, byte_read);
		if (byte_read == -1)
		{
			std::cerr << "sortie recv -1" << std::endl;
		}
		if (byte_read == 0)
		{
			std::cerr << "sortie recv 0" << std::endl;
		}

	} while (byte_read > 0);

	return url_decode(return_str);
}

void	creat_client(monitoring &moni, int &fd, char** env)
{
	(void)env;
	client *new_client = NULL;

	int tmp_fd_client = accept(fd, NULL, NULL);
	if (tmp_fd_client < 0)
		throw std::runtime_error(RED "error from accept");

	std::cout << ORANGE "creat client fd : " << tmp_fd_client << " from serveur : " << fd <<  RESET << std::endl;

	new_client = new client(tmp_fd_client, 11111, true);
	if (!new_client)
		throw std::runtime_error(RED "error from 'new clien'");

	moni.clients[new_client->getFD()] = new_client;
	moni.all_all_pollfd.push_back(new_client->clien_pollfd);
}

void	read_client(monitoring &moni, int &fd, int i)
{
	(void)i;
	std::cout << "Client prêt à etre lu fd : " << fd << std::endl;
	std::string read_text = read_request(fd);

	std::map<int, client *>::iterator it = moni.clients.find(fd);

	if (it == moni.clients.end() && !read_text.empty())
	{
		int tmp_fd = moni.where_are_fd_pipe(fd);
		std::cout << "client cgi ok read fd " << fd <<  "tmp : " << tmp_fd << read_text << std::endl;

		ssize_t bytes_sent = send(tmp_fd, read_text.c_str(), read_text.size(), 0);
		if (bytes_sent == -1)
			throw std::runtime_error(RED "Erreur lors de l'envoi des données dans read_client");

		moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
		delete moni.clients[tmp_fd];
		moni.clients.erase(tmp_fd);
	}
	else if (!read_text.empty() && it != moni.clients.end())
	{
		moni.clients[fd]->setInput(read_text);
		std::cout << RED "input from read_client  fd : " << moni.clients[fd]->getFD() << " " << read_text << RESET << std::endl;
	}
}

void	responding(monitoring &moni, int &fd, char **env, int i)
{
	// std::cout << "Client prêt à recevoir fd : " << fd << std::endl;

	std::map<int, client*>::iterator it = moni.clients.find(fd);

	if (it == moni.clients.end())
	{
		std::cout << "client cgi ok write fd " << fd << std::endl;
		int tmp_fd = moni.where_are_fd_pipe(fd);
		if (tmp_fd == -1)
			std::cerr << "error tmp_fd where_are_fd_pipe" << std::endl;

		std::cout << "tmp : " << tmp_fd << "input : " << (*moni.clients[tmp_fd]).getInput().size() << std::endl;

		ssize_t bytes_sentt = write(fd, (*moni.clients[tmp_fd]).getInput().c_str(), (*moni.clients[tmp_fd]).getInput().size());
		if (bytes_sentt == -1)
			throw std::runtime_error(RED "write Erreur lors de l'envoi des données pour cgi dans responding");

		std::cout << "write ok dans pipe" << std::endl;

		// close(moni.clients[tmp_fd]->pipe_write[1]);
		// close(moni.all_all_pollfd[i].fd);
		moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
	}
	else if (!moni.clients[fd]->getStatusCgi())
	{
		if (!(*moni.clients[fd]).getInput().empty() && !(*moni.clients[fd]).getStatusCgi())
			raph(moni, *moni.clients[fd], env);

		if (!(*moni.clients[fd]).getStatusCgi())
		{
			std::cout << ORANGE "Input from responding fd " << (*moni.clients[fd]).getFD() << "= " << BLUE << (*moni.clients[fd]).getInput() << RESET << std::endl;

			ssize_t bytes_sent = send((*moni.clients[fd]).getFD(), (*moni.clients[fd]).getOutput().c_str(), (*moni.clients[fd]).getOutput().size(), 0);
			if (bytes_sent == -1)
				throw std::runtime_error(RED "Erreur lors de l'envoi des données dans responding");
			std::cout << GREEN "Réponse : OK" RESET << std::endl;

			delete moni.clients[fd];
			moni.clients.erase(fd);
			moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
		}
	}
}

void	error(monitoring &moni, pollfd &poll, int i)
{
	std::cout << "dans error fd : " << poll.fd << std::endl;
	client *cl = moni.clients[poll.fd];
	std::map<int, client *>::iterator it = moni.clients.find(poll.fd);

	if (it != moni.clients.end() && cl)
	{
		if(compar(cl->getFD(), moni.all_pollfd_servor))
			std::cout <<RED << "is a servor" << RESET << std::endl;
	}
	else if (poll.revents & POLLERR)
		std::cout <<RED << "POLLERR" << RESET << std::endl;
	else if (poll.revents & POLLHUP)
		std::cout <<RED << "POLLHUP" << RESET << std::endl;
	else if (poll.revents & POLLNVAL)
		std::cout <<RED << "POLLNVAL" << RESET << std::endl;

	delete moni.clients[poll.fd];
	moni.clients.erase(poll.fd);
	moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
}

void	raph(monitoring &moni, client &cl, char** env)
{
	/* reponce */
	RequestIn test(cl.getInput(), env);
    // std::cout << test.getCode() << std::endl;
    // std::cout << "---------------" << std::endl;
    if (test.getCode() == 200) {
        test.checkErrorHTTPHeaders();
        std::cout << test.getCode() << std::endl;
    }
    // std::cout << test.getCode() << std::endl;
    if (test.getCode() == 200) {
        test.checkErrorHTTPBody();
        std::cout << test.getCode() << std::endl;
    }
    if (test.getCode() == 200) {
        test.parseBody();
        // std::cout << test.getCode() << std::endl;
    }
	std::string response = test.makeResponse(moni, cl);

	cl.setOutput(response);
}

bool compar(const int fd, const std::vector<pollfd> &poll_servor)
{
	if (fd)
	{
		for (int i = 0; i < (int)poll_servor.size(); ++i)
		{
			if (fd == poll_servor[i].fd)
				return true;
		}
	}
	return false;
}
