/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/11 15:37:21 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"
#include "../get_next_line/get_next_line.h"

client::client(int fdsocket) : socket_fd(fdsocket)
{

	this->pfd.fd = this->socket_fd;
	this->pfd.events = POLLIN;
	this->pfd.revents = 0;

	std::cout << GREEN "creat client fd : " << this->socket_fd << RESET << std::endl;
}

client::~client()
{
	close(this->socket_fd);

	std::cout << RED "delete client fd : " << this->socket_fd << RESET << std::endl;
}


std::string read_request(int &fd_client)
{
	std::string	string = NULL;
	char 		buff[100];
	short		byte_read;

	do
	{
		std::fill(buff, buff + sizeof(buff), 0);
		byte_read = read(fd_client, buff, sizeof(buff));
		string = ft_str_join(string, buff, ft_strlen(buff));
		std::cout << "buff = " << buff;
		std::cout << "byte = " << byte_read;
	} while (byte_read > 0);
	
	write(1, "ok\n", 3);
	std::cout << "okkk" << std::endl;

	std::string return_str(string);

	free(string);

	return return_str;
} 

client	*creat_client(int fd_serveur)
{
	int tmp_fd_client = accept(fd_serveur, NULL, NULL);
	if (tmp_fd_client < 0)
	{
		std::cerr << RED "error creat client" RESET << std::endl;
		return NULL;
	}
	client* new_client = new client(tmp_fd_client);
	if (!new_client)
		return NULL;

	new_client->input = read_request(tmp_fd_client);

	const char *message = "HTTP/1.1 200 OK\r\n"
						  "Content-Type: text/html\r\n"
						  "Content-Length: 32\r\n"
						  "Connection: close\r\n"
						  "\r\n"
						  "<html><body><h1>Hello from server!</h1></body></html>";

	// Envoi du message au client
	ssize_t bytes_sent = send(tmp_fd_client, message, strlen(message), 0);
	if (bytes_sent == -1)
	{
		std::cerr << "Erreur lors de la lecture" << std::endl;
		return NULL;
	}

	return new_client;
}
