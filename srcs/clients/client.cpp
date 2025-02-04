/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/04 09:57:51 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"
#include "../../includes/Parser.hpp"

/* -------------------------------------------------------- */
/* --------------CONSTRUCTOR / DESTRUCTOR------------------ */
/* -------------------------------------------------------- */

client::client(int fdsocket) : socket_fd(fdsocket)
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
	close(this->socket_fd);

	std::cout << RED "delete client fd : " << this->socket_fd << RESET << std::endl;
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

	do
	{
		std::memset(buff, 0, sizeof(buff));
		byte_read = read(fd_client, buff, sizeof(buff));

		int actual_flag = fcntl(fd_client, F_GETFL, 0);
		fcntl(fd_client, F_SETFL, actual_flag | O_NONBLOCK);

		if (byte_read > 0)
            return_str.append(buff, byte_read);

	} while (byte_read > 0);

	std::cout << RED "return str" RESET << return_str << std::endl;
	return url_decode(return_str);
}



void creat_client(serveur &servor, char** env)
{
	client *new_client = NULL;

	std::cout << ORANGE "server listen . . ." RESET << std::endl;
	int timlaps = -1;
	if (!servor.client.empty())
		timlaps = 1000;

	int readpoll = poll(&servor.pfd, 1, timlaps);
	if (readpoll < 0)
		throw std::runtime_error(RED "Error poll = -1");
	if (servor.pfd.revents & POLLIN)
	{
		int tmp_fd_client = accept(servor.getFD(), NULL, NULL);
		write(2, "OK1\n", 4);
		if (tmp_fd_client < 0)
			throw std::runtime_error(RED "error creat client");

		write(2, "OK2\n", 4);
		std::cout << ORANGE "creat client" RESET << std::endl;

		new_client = new client(tmp_fd_client);
		write(2, "OK3\n", 4);

		new_client->setInput(read_request(new_client->getFD()));
		write(2, "OK4\n", 4);
		new_client->setOutput(raph(new_client->getInput(), env));
		std::cout << RED << new_client->getOutput() << RESET << std::endl;
		servor.client.push_back(new_client);
	}
}

void	responding(serveur &servor)
{
	if (!servor.client.empty())
	{
		std::vector<client*>::iterator	cl = servor.client.begin();
		int readpoll  = poll(&(*cl)->clien_pollfd, 1, 300);
		if (readpoll < 0)
			throw std::runtime_error(RED "Error poll = -1");
		
		if ((*cl)->clien_pollfd.revents & POLLOUT)
		{
			std::cout <<ORANGE "input = " BLUE << (*cl)->getInput() << RESET << std::endl;

			ssize_t bytes_sent = send((*cl)->getFD(), (*cl)->getOutput().c_str(), (*cl)->getOutput().size(), 0);
			if (bytes_sent == -1)
			{
				std::cerr << "Erreur lors de la lecture" << std::endl;
			}
			std::cout << GREEN "reponse : ok" RESET << std::endl;
			delete *cl;
			servor.client.erase(cl);
		}
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
