/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 11:46:12 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/02 14:49:30 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"
#include "../../includes/Parser.hpp"

std::string client::read_request(const int &fd_client)
{
	std::string return_str;
	char buff[4096];
	short byte_read;
	size_t content_lenght_pos = 0;

	do
	{
		std::memset(buff, 0, sizeof(buff));
		byte_read = recv(fd_client, buff, sizeof(buff) - 1, MSG_DONTWAIT);
		if (byte_read == -1)
		{
			std::cerr << "sortie recv -1" << std::endl;
			return return_str;
		}
		else if (byte_read == 0)
		{
			std::cerr << "sortie recv 0" << std::endl;
			this->client_close = true;
			return return_str;
		}
		else if (byte_read > 0)
		{
			return_str.append(buff, byte_read);
			if (!this->first_pass)
			{
				content_lenght_pos = return_str.find("Content-Length: ");
				if (content_lenght_pos != std::string::npos)
				{
					content_lenght_pos += 16;
					size_t end_pos = return_str.find("\r\n\r\n", content_lenght_pos);
					this->content_lenght = std::atoi(return_str.substr(content_lenght_pos, end_pos - content_lenght_pos).c_str());
					this->first_pass = true;
					this->content_lenght += end_pos + 4;
				}
			}
			this->content_real += byte_read;
		}

	} while (byte_read > 0);

	return return_str;
}

std::string client::read_request_cgi(const int &fd_client)
{
	std::string return_str;
	char buff[4096];
	short byte_read;
	size_t content_lenght_pos = 0;

	do
	{
		std::memset(buff, 0, sizeof(buff));
		byte_read = read(fd_client, buff, sizeof(buff));
		if (byte_read == -1)
		{
			std::cerr << "sortie read -1" << std::endl;
			return return_str;
		}
		else if (byte_read == 0)
		{
			std::cerr << "sortie read 0" << std::endl;
			this->client_close_cgi = true;
			return return_str;
		}
		else if (byte_read > 0)
		{
			return_str.append(buff, byte_read);
			if (!this->first_pass)
			{
				content_lenght_pos = return_str.find("Content-Length: ");
				if (content_lenght_pos != std::string::npos)
				{
					this->first_pass = true;
					content_lenght_pos += 16;
					size_t end_pos = return_str.find("\r\n\r\n", content_lenght_pos);
					this->content_lenght = std::atoi(return_str.substr(content_lenght_pos, end_pos - content_lenght_pos).c_str());
					this->content_lenght += end_pos + 4;
				}
			}
			this->content_real += byte_read;
		}
	} while (byte_read > 0);

	return return_str;
}

void read_client(monitoring &moni, int &fd, int i)
{
	// std::cout << "Client prêt à etre lu fd : " << fd << std::endl;
	std::map<int, client *>::iterator it = moni.clients.find(fd);

	if (it == moni.clients.end())
	{
		int tmp_fd = moni.where_are_fd_pipe(fd);
		if (tmp_fd == -1)
			std::cerr << "throw error where_are_fd_pipe" << std::endl;

		std::string read_text = moni.clients[tmp_fd]->read_request_cgi(fd);

		// std::cout << "client cgi ok read fd " << fd << "tmp : " << tmp_fd << read_text << std::endl;

		moni.clients[tmp_fd]->setOutput(read_text);

		if ((moni.clients[tmp_fd]->client_close_cgi || (moni.clients[tmp_fd]->content_lenght > 0 &&
			 moni.clients[tmp_fd]->content_real >= moni.clients[tmp_fd]->content_lenght)))
		{
			std::cout << RED "close read pipe pipe read fd : " << tmp_fd  << " " << fd << RESET << std::endl;
			close((*moni.clients[tmp_fd]).pipe_read[0]);
			moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
			moni.clients[tmp_fd]->responce_cgi = true;
			if (moni.clients[tmp_fd]->client_close_cgi)
				moni.clients[tmp_fd]->AddOutput(timeout(*moni.clients[tmp_fd], moni, "502", Initer::initMapConfig(moni, tmp_fd)));
		}
	}
	else
	{
		std::string read_text = moni.clients[fd]->read_request(fd);
		if (moni.clients[fd]->client_close)
		{
			delete_client(moni, fd, i);
			return;
		}
		else
			moni.clients[fd]->setInput(read_text);
		// std::cout << RED "input from read_client  fd : " << moni.clients[fd]->getFD() << " " << read_text << RESET << std::endl;
	}
}
