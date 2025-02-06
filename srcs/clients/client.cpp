/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/28 15:43:35 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"

client::client()
{
	
}
client::client(int fdsocket) : socket_fd(fdsocket)
{
	this->pfd.fd = this->socket_fd;
	this->pfd.events = POLLIN;
	this->pfd.revents = 0;

	this->startTime = std::time(NULL);
	if (this->startTime == -1)
		throw std::runtime_error(RED "Failed to get the current time" RESET);

	std::cout << GREEN "creat client fd : " << this->socket_fd << "time = " << this->startTime << RESET << std::endl;
}

client::~client()
{
	close(this->socket_fd);

	std::cout << RED "delete client fd : " << this->socket_fd << RESET << std::endl;
}

int	client::getFD()
{
	return this->socket_fd;
}
	std::string read_request(int &fd_client)
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

	return return_str;
} 

void	creat_client(int fd_serveur, serveur &servor)
{
	std::cout << ORANGE "creat client" RESET << std::endl;

	int tmp_fd_client = accept(fd_serveur, NULL, NULL);

	if (tmp_fd_client < 0)
		throw std::runtime_error(RED "error creat client");

	client *new_client = new client(tmp_fd_client);

	servor.clients[tmp_fd_client] = new_client;
	servor.clients[tmp_fd_client]->input = read_request(tmp_fd_client); 

	std::cout << servor.clients[tmp_fd_client]->input << std::endl;

	for (int i = 1; i <= MAX_CLIENTS; i++)
	{
		std::cout << "ok" << std::endl;
		if (servor.pfd[i].fd == -1)
		{
			std::cout << "okkk" << std::endl;
			servor.pfd[i].fd = new_client->getFD();
			// servor.pfd[i].events = POLLIN;
			break;
		}
	}

	/* reponce */

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
    	send(tmp_fd_client, error_response.c_str(), error_response.size(), 0);
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

	// Envoi du message au client
	ssize_t bytes_sent = send(tmp_fd_client, message.c_str(), message.size(), 0);
	if (bytes_sent == -1)
	{
		std::cerr << "Error lors de la lecture" << std::endl;
	}

}

void	client_existed(int &fd_client, serveur &servor)
{
	std::cout << ORANGE "client existed" RESET << std::endl;

	servor.clients[fd_client]->input = read_request(fd_client);

	std::cout << servor.clients[fd_client]->input << std::endl;



		/* reponce */

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
		send(fd_client, error_response.c_str(), error_response.size(), 0);
	}

	std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	std::ostringstream oss;
	oss << file_content.size();
	std::string content_length = oss.str();

	std::string message =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: " +
		content_length + "\r\n"
						 "\r\n" +
		file_content;

	// Envoi du message au client
	ssize_t bytes_sent = send(fd_client, message.c_str(), message.size(), 0);
	if (bytes_sent == -1)
	{
		std::cerr << "Error lors de la lecture" << std::endl;
	}
}
