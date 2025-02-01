/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/02/01 11:48:42 by omfelk           ###   ########.fr       */
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

std::string url_decode(const std::string &url) {
    std::string decoded = "";
    for (size_t i = 0; i < url.length(); ++i) {
        if (url[i] == '%' && i + 2 < url.length()) {
            std::string hex = url.substr(i + 1, 2);
            char decoded_char = 0;
            if ((hex[0] >= '0' && hex[0] <= '9') || (hex[0] >= 'A' && hex[0] <= 'F') || (hex[0] >= 'a' && hex[0] <= 'f')) {
                if ((hex[1] >= '0' && hex[1] <= '9') || (hex[1] >= 'A' && hex[1] <= 'F') || (hex[1] >= 'a' && hex[1] <= 'f')) {
                    decoded_char = static_cast<char>(::strtol(hex.c_str(), NULL, 16)); // Décoder l'hexadécimal
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

void	     creat_client(int fd_serveur)
{
	std::cout << ORANGE "creat client" RESET << std::endl;

	int tmp_fd_client = accept(fd_serveur, NULL, NULL);
	if (tmp_fd_client < 0)
		throw std::runtime_error(RED "error creat client");

	client	new_client = client(tmp_fd_client);

	new_client.input = read_request(tmp_fd_client); 

	std::cout << url_decode(new_client.input) << std::endl;
	//std::cout << new_client.input << std::endl;

	/* reponce */

	char path[] = "./html/errors/errors_400/error_404.html";
	//char path[] = "./html/test.html";

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
		std::cerr << "Erreur lors de la lecture" << std::endl;
	}

}

//void	client_existed(int &fd_client, serveur &servor)
//{
//	std::cout << ORANGE "client existed" RESET << std::endl;

//	servor.clients[fd_client]->input = read_request(fd_client);

//	std::cout << servor.clients[fd_client]->input << std::endl;



//		/* reponce */

//	char path[] = "./html/index.html";

//	std::ifstream file(path);
//	if (!file.is_open())
//	{
//		std::string error_response =
//			"HTTP/1.1 404 Not Found\r\n"
//			"Content-Type: text/plain\r\n"
//			"Content-Length: 13\r\n"
//			"\r\n"
//			"404 Not Found";
//		send(fd_client, error_response.c_str(), error_response.size(), 0);
//	}

//	std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//	file.close();

//	std::ostringstream oss;
//	oss << file_content.size();
//	std::string content_length = oss.str();

//	std::string message =
//		"HTTP/1.1 200 OK\r\n"
//		"Content-Type: text/html\r\n"
//		"Content-Length: " +
//		content_length + "\r\n"
//						 "\r\n" +
//		file_content;

//	// Envoi du message au client
//	ssize_t bytes_sent = send(fd_client, message.c_str(), message.size(), 0);
//	if (bytes_sent == -1)
//	{
//		std::cerr << "Erreur lors de la lecture" << std::endl;
//	}
//}
