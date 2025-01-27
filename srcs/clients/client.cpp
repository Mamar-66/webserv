/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/27 14:46:12 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"

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

client	*creat_client(int fd_serveur)
{
	int tmp_fd_client = accept(fd_serveur, NULL, NULL);
	std::cout << "tmp fd " << tmp_fd_client << std::endl;
	if (tmp_fd_client < 0)
	{
		std::cerr << RED "error creat client" RESET << std::endl;
		return NULL;
	}
	client* new_client = new client(tmp_fd_client);
	if (!new_client)
		return NULL;

	new_client->input = read_request(tmp_fd_client);

	char path[] = "/home/omfelk/42/cursus/c5/webserv/webserv_home/html/index.html";

	std::ifstream file(path);
	if (!file.is_open()) {
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
    	"Content-Length: " + content_length + "\r\n"
    	"\r\n" +
    	file_content;

	// Envoi du message au client
	ssize_t bytes_sent = send(tmp_fd_client, message.c_str(), message.size(), 0);
	if (bytes_sent == -1)
	{
		std::cerr << "Erreur lors de la lecture" << std::endl;
		return NULL;
	}

	return new_client;
}
