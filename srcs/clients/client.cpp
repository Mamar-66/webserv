/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/10 13:45:33 by omfelk           ###   ########.fr       */
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
	this->clien_pollfd.events = POLLOUT;
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

std::string read_request(const int &fd_client)
{
	std::string	return_str;
	char 		buff[2048];
	short		byte_read;

    int flags = fcntl(fd_client, F_GETFL, 0);
    if (flags == -1)
	{
		std::cerr << "error from fcntl flag" << std::endl;
		return "-1";
	}
	flags |= O_NONBLOCK;
    if (fcntl(fd_client, F_SETFL, flags) == -1)
	{
        std::cerr << "error from fcntl change flag" << std::endl;
		return "-1"; 
	}

	do
	{
		std::memset(buff, 0, sizeof(buff));
		byte_read = read(fd_client, buff, sizeof(buff));
		if (byte_read < 0)
			std::cerr << RED "Error from read" RESET << std::endl;

		if (byte_read > 0)
            return_str.append(buff, byte_read);

	} while (byte_read > 0);

	return url_decode(return_str);
}

void creat_client(serveur &servor, char** env)
{
	client *new_client = NULL;

	int tmp_fd_client = accept(servor.getFD(), NULL, NULL);
	if (tmp_fd_client < 0)
		throw std::runtime_error(RED "error frem accept");

	std::cout << ORANGE "creat client" RESET << std::endl;

	std::string read_text = read_request(tmp_fd_client);
	if (read_text == "-1")
		return;

	new_client = new client(tmp_fd_client, servor.client_max_body_size, true);
	if (!new_client)
		throw std::runtime_error(RED "error from 'new clien'");

	if (read_text.empty())
	{
		delete new_client;
		return;
	}
	new_client->setInput(read_text);
	new_client->setOutput(raph(new_client->getInput(), env));

	servor.clients[new_client->getFD()] = new_client;
	servor.all_pollfd.push_back(new_client->clien_pollfd);
}

void responding(serveur &servor, int &fd)
{
    std::cout << "Client prêt à recevoir" << std::endl;

    client *cl = servor.clients[fd];
    std::string output = cl->getOutput();

    if (!cl->getStatusCgi())
    {
        std::cout << ORANGE "Input = " BLUE << cl->getInput() << RESET << std::endl;

        ssize_t bytes_sent = send(cl->getFD(), output.c_str(), output.size(), 0);
        if (bytes_sent == -1)
        {
            throw std::runtime_error(RED "Erreur lors de l'envoi des données");
        }
        std::cout << GREEN "Réponse : OK" RESET << std::endl;

        std::vector<pollfd>::iterator it = servor.all_pollfd.begin();

        for (; it != servor.all_pollfd.end(); ++it)
        {

            if (it->fd == cl->getFD())
            {
                servor.all_pollfd.erase(it);
                break;
            }
        }
        delete servor.clients[fd];
        servor.clients.erase(fd);
	}
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
