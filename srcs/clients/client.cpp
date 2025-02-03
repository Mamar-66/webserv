/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/03 16:47:14 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"

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

	return url_decode(return_str);
} 


void	creat_client(serveur &servor)
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
		if (tmp_fd_client < 0)
			throw std::runtime_error(RED "error creat client");

		std::cout << ORANGE "creat client" RESET << std::endl;

		new_client = new client(tmp_fd_client);

		new_client->setInput(read_request(new_client->getFD()));
		servor.client.push_back(new_client);
	}


	/* reponce */

	// char path[] = "./html/errors/errors_400/error_404.html";
	//char path[] = "./html/test.html";
	char path[] = "./html/index.html";

	std::ifstream file(path);
	if (!file.is_open())
	{
    	std::string error_response = 
    	    "HTTP/1.1 404 Not Found\r\n"
    	    "Content-Type: text/plain\r\n"
    	    "Content-Length: 13\r\n"
    	    "\r\n"
    	    "404 Not Found";
    	send(new_client->getFD(), error_response.c_str(), error_response.size(), 0);
	}

	std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

	std::ostringstream oss;
    oss << file_content.size();
    std::string content_length = oss.str();

	std::string message =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Connection: keep-alive\r\n"
		"Keep-Alive: timeout: 15, max: 100\r\n"
		"Content-Length: " + content_length + "\r\n"
		"\r\n" + file_content;

	/* ---------- fin reponse ---------------- */
	if (new_client)
		new_client->setOutput(message); // put send in class

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
