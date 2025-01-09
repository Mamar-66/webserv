/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:29:52 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/09 20:25:27 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.hpp"

int main()
{
	try 
	{
		routine_servor();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 1;
}

// #include <iostream>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <vector>
// #include <poll.h>
// #include <cstring>

// #define PORT 8080 

// int main() {

// 	std::vector<pollfd> vec;

// 	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (server_fd == -1)
// 	{
// 		perror("error creat socket");
// 		return 1;
// 	}

// 	pollfd pfd = {
// 		server_fd, POLLIN, 0
// 	};

// 	vec.push_back(pfd);

// 	sockaddr_in srv = {};
// 	srv.sin_family = AF_INET;
// 	srv.sin_addr.s_addr = INADDR_ANY;
// 	srv.sin_port = htons(PORT);

// 	if ( bind(server_fd, (struct sockaddr*)&srv, sizeof(srv)) < 0)
// 	{
// 		perror("error bind");
// 		close(server_fd);
// 		return 1;
// 	}

// 	if (listen(server_fd, 10) < 0)
// 	{
// 		perror("error listen");
// 		close(server_fd);
// 		return 1;
// 	}

// 	std::cout << "serveur listen" << std::endl;

// 	while (true)
// 	{
// 		if (poll(&pfd, 1, -1) >= 0 && pfd.revents == POLLIN)
// 		{
// 			int client_fd = accept(server_fd, nullptr, nullptr);
// 			if (client_fd < 0)
// 			{
// 				perror("error accept");
// 				continue;
// 			}

// 			std::cout << "nwe clien ok " << client_fd << std::endl;

// 			char buffer[4096] = {};  // Tampon pour lire les données du client
//             ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);  // -1 pour laisser de la place pour le '\0'
//             if (bytes_read < 0) {
//                 perror("Erreur lors de la lecture");
//                 close(client_fd);
//                 continue;
//             }

// 			std::cout << "Données reçues du client:\n" << buffer << std::endl;

// 			    const char *message = "HTTP/1.1 200 OK\r\n"
//         								"Content-Type: text/html\r\n"
//         								"Content-Length: 32\r\n"
//         								"Connection: close\r\n"
//         								"\r\n"
//         								"<html><body><h1>Hello from server!</h1></body></html>";
    
//    				 // Envoi du message au client
//    				 ssize_t bytes_sent = send(client_fd, message, strlen(message), 0);
//    				 if (bytes_sent == -1) {
//    				     perror("Erreur lors de l'envoi");
//    				     close(server_fd);
//    				     return 1;
//    				 }

// 		}
// 		else
// 			perror("error poll");

// 	}

// 	close(server_fd);

//     return 0;
// }
