/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/13 17:19:36 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"
#include "../../includes/Parser.hpp"

/* -------------------------------------------------------- */
/* --------------CONSTRUCTOR / DESTRUCTOR------------------ */
/* -------------------------------------------------------- */

client::client(int fdsocket, size_t size_body, bool listing) : 
socket_fd(fdsocket), size_body(size_body), listing(listing), is_cgi(false)
{
	this->startTime = std::time(NULL);
	if (this->startTime == -1)
		throw std::runtime_error(RED "Failed to get the current time" RESET);

	this->clien_pollfd.fd = this->socket_fd;
	this->clien_pollfd.events = POLLIN | POLLOUT;
	this->clien_pollfd.revents = 0;

	std::cout << GREEN "creat client fd : " << this->socket_fd << "time = " << this->startTime << RESET << std::endl;
}

client::~client()
{
	if (this->socket_fd != -1)
	{
		close(this->socket_fd);
		std::cout << RED "delete client whith close fd : " << this->socket_fd << RESET << std::endl;
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

void	client::setStatusCgiTrue()
{
	this->is_cgi = true;
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

	do
	{
		std::memset(buff, 0, sizeof(buff));
		byte_read = recv(fd_client, buff, sizeof(buff), MSG_DONTWAIT);

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

	new_client = new client(tmp_fd_client, 1111111, true);
	if (!new_client)
		throw std::runtime_error(RED "error from 'new clien'");

	moni.clients[new_client->getFD()] = new_client;
	moni.all_all_pollfd.push_back(new_client->clien_pollfd);
}

void	read_client(monitoring &moni, int &fd)
{
	std::cout << "Client prêt à etre lu" << std::endl;

	std::string read_text = read_request(fd);

	if (!read_text.empty())
	{
		moni.clients[fd]->setInput(read_text);
		std::cout << RED "input : " << read_text  << RESET << std::endl;
	}
}

void	responding(monitoring &moni, int &fd, char **env, int i)
{
    std::cout << "Client prêt à recevoir fr : " << fd << std::endl;

    client *cl = moni.clients[fd];

    if (!cl->getStatusCgi() && !cl->getInput().empty())
    {
    std::cout << ORANGE "Input = " BLUE << cl->getInput() << RESET << std::endl;

		cl->setOutput(raph(cl->getInput(), env));

		ssize_t bytes_sent = send(cl->getFD(), cl->getOutput().c_str(), cl->getOutput().size(), 0);
		if (bytes_sent == -1)
            throw std::runtime_error(RED "Erreur lors de l'envoi des données");
        std::cout << GREEN "Réponse : OK" RESET << std::endl;

        delete moni.clients[fd];
        moni.clients.erase(fd);
		moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
	}
}

void	error(monitoring &moni, pollfd &poll, int i)
{
	std::cout << "dans error fd : " << poll.fd << std::endl;
	client *cl = moni.clients[poll.fd];

	if (compar(cl->getFD(), moni.all_pollfd_servor))
		std::cout <<RED << "is a servor" << RESET << std::endl;
	if (poll.revents & POLLERR)
		std::cout <<RED << "POLLERR" << RESET << std::endl;
	if (poll.revents & POLLHUP)
		std::cout <<RED << "POLLHUP" << RESET << std::endl;
	if (poll.revents & POLLNVAL)
		std::cout <<RED << "POLLNVAL" << RESET << std::endl;

	delete moni.clients[poll.fd];
	moni.clients.erase(poll.fd);
	moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
}

std::string	raph(const std::string& input, char** env)
{
	/* reponce */
	RequestIn test(input, env);
    std::cout << test.getCode() << std::endl;
    std::cout << "---------------" << std::endl;
    if (test.getCode() == 200) {
        test.checkErrorHTTPHeaders();
        std::cout << test.getCode() << std::endl;
    }
    std::cout << test.getCode() << std::endl;
    if (test.getCode() == 200) {
        test.checkErrorHTTPBody();
        std::cout << test.getCode() << std::endl;
    }
    if (test.getCode() == 200) {
        test.parseBody();
        std::cout << test.getCode() << std::endl;
    }
    std::string response = test.makeResponse();
	return response;
}

bool compar(const int &fd, const std::vector<pollfd> &poll_servor)
{
	for (int i = 0; i < (int)poll_servor.size(); ++i)
	{
		if (fd == poll_servor[i].fd)
			return true;
	}
	return false;
}
