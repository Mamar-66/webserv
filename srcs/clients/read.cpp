/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmichel- <rmichel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 11:46:12 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/13 09:42:27 by rmichel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Webserv.h"

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
			return return_str;
		}
		else if (byte_read == 0)
		{
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
			return return_str;
		}
		else if (byte_read == 0)
		{
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
	std::map<int, client *>::iterator it = moni.clients.find(fd);

	if (fd > 0)
	{
		if (it == moni.clients.end())
		{
			int tmp_fd = moni.where_are_fd_pipe(fd);
			if (tmp_fd == -1)
				std::cerr << "throw error where_are_fd_pipe" << std::endl; // NO THROW / ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			std::string read_text = moni.clients[tmp_fd]->read_request_cgi(fd);

			moni.clients[tmp_fd]->setOutput(read_text);

			if (((moni.clients[tmp_fd]->content_lenght > 0 &&
				moni.clients[tmp_fd]->content_real >= moni.clients[tmp_fd]->content_lenght)))
			{
				close((*moni.clients[tmp_fd]).pipe_read[0]);
				moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
				moni.clients[tmp_fd]->responce_cgi = true;
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
		}
	}
}
