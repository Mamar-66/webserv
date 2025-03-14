/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmichel- <rmichel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/13 09:40:45 by rmichel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Webserv.h"

/* -------------------------------------------------------- */
/* --------------CONSTRUCTOR / DESTRUCTOR------------------ */
/* -------------------------------------------------------- */

client::client(client& cpy) {
	(void)cpy;
}
client& client::operator=(client& cpy) {
	(void)cpy;
	return *this;
}

client::client(int fdsocket, int fdFather) :
socket_fd(fdsocket), socket_fd_father(fdFather),
is_cgi(false), content_lenght(0), chunked(false), first_pass(false),
responce_cgi(false), client_close(0), content_real(0), pid_child(0)
{
	this->timeout = false;
	this->startTime = std::time(NULL);
	if (this->startTime == -1)
		throw std::runtime_error(RED "Failed to get the current time" RESET);

	this->envp = NULL;

	this->clien_pollfd.fd = this->socket_fd;
	this->clien_pollfd.events = POLLIN | POLLOUT;
	this->clien_pollfd.revents = 0;

	std::cerr << GREEN "creat client fd : " << this->socket_fd << "time = " << this->startTime << RESET << std::endl; /* To keep */
}

client::~client()
{
	if (this->socket_fd != -1)
	{
		close(this->socket_fd);
		std::cerr << RED "delete client whith close fd : " << this->socket_fd << RESET << std::endl; /* To keep */

		if (this->getStatusCgi())
		{
			if (this->envp)
			{
				for(int i = 0; this->envp[i]; ++i)
					delete [] this->envp[i];
				delete[] this->envp;
			}

			close(this->pipe_write[0]);
			close(this->pipe_write[1]);
			close(this->pipe_read[0]);
			close(this->pipe_read[1]);
		}
	}
	else
		std::cerr << RED "delete client warning don't close fd : " << this->socket_fd << RESET << std::endl; /* To keep */
}

/* -------------------------------------------------------- */
/* -------------------- GETTER -----------------------------*/
/* -------------------------------------------------------- */

const int	&client::getFD()
{
	return this->socket_fd;
}

const int &client::getFDFather()
{
	return this->socket_fd_father;
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

const bool	&client::getTimeoutBool( void ) {
	return this->timeout;
}

const std::string &client::getGoodLocation( void ) {
	return this->location;
}

/* -------------------------------------------------------- */
/* -------------------- SETTER ---------------------------- */
/* -------------------------------------------------------- */

void	client::setInput(const std::string& str)
{
	this->input.append(str);
	if (str != "\n")
		this->setActualTime();
}

void	client::AddInput(const std::string &str)
{
	this->input = str;
	this->setActualTime();
}

void	client::setOutput(const std::string& str)
{
	this->output.append(str);
	this->setActualTime();
}

void	client::AddOutput(const std::string &str)
{
	this->output = str;
}

void	client::setCgiTrue()
{
	this->is_cgi = true;
}
void client::setCgiFalse()
{
	this->is_cgi = false;
}

void	client::setActualTime()
{
	this->startTime = std::time(NULL);
}

void	client::setEnv(char **env)
{
	this->envp = env;
}

void client::setTimeoutBool( bool state ) {
	this->timeout = state;
}

void client::setGoodLocation( std::string Location) {
	this->location = Location;
}
/* --------------------------------------------------- */
/* --------------------------------------------------- */
/* --------------------------------------------------- */

void	delete_client(monitoring &moni, int fd, int i)
{
	delete moni.clients[fd];
	moni.clients.erase(fd);
	moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
}

void	creat_client(monitoring &moni, int &fd)
{
	client *new_client = NULL;

	int tmp_fd_client = accept(fd, NULL, NULL);
	if (tmp_fd_client < 0)
		std::cerr << RED "error from accept" RESET << std::endl;
	else
	{
		// std::cerr << ORANGE "creat client fd : " << tmp_fd_client << " from serveur : " << fd <<  RESET << std::endl; /* Remove if you want to see which server created the connexion */

		new_client = new client(tmp_fd_client, fd);
		if (!new_client)
		{
			std::cerr << RED "error from 'new clien' close fd : " << tmp_fd_client << RESET << std::endl;
			close(tmp_fd_client);
		}
		else
		{
			moni.clients[new_client->getFD()] = new_client;
			moni.all_all_pollfd.push_back(new_client->clien_pollfd);
		}
	}
}

void	responding(monitoring &moni, int &fd, int i)
{
	std::map<int, client*>::iterator it = moni.clients.find(fd);

	if (it == moni.clients.end())
	{
		int tmp_fd = moni.where_are_fd_pipe(fd);
		if (tmp_fd == -1)
			std::cerr << RED "error tmp_fd where_are_fd_pipe" RESET << std::endl;

		// std::cerr << BLUE << "tmp : " << ORANGE << tmp_fd << std::endl << std::endl << BLUE << "input : " std::endl << ORANGE << (*moni.clients[tmp_fd]).getInput() << RESET << std::endl; /* Uncomment if you want to see the request */ 

		(*moni.clients[tmp_fd]).setInput("\n");

		ssize_t bytes_sentt = write(fd, (*moni.clients[tmp_fd]).getInput().c_str(), (*moni.clients[tmp_fd]).getInput().size());
		if (bytes_sentt == -1)
		{
			std::cerr << RED "Write Erreur data in FD Pipe delete client" RESET << std::endl;
			close((*moni.clients[tmp_fd]).pipe_write[1]);
			close((*moni.clients[tmp_fd]).pipe_write[0]);
			moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
			moni.clients[tmp_fd]->AddOutput(timeout(*moni.clients[tmp_fd], moni, "502", Initer::initMapConfig(moni, tmp_fd)));
		}
		else if (bytes_sentt == 0)
		{
			std::cerr << RED "write Erreur Pipe delete client" RESET << std::endl;
			close((*moni.clients[tmp_fd]).pipe_write[1]);
			close((*moni.clients[tmp_fd]).pipe_write[0]);
			moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
			moni.clients[tmp_fd]->AddOutput(timeout(*moni.clients[tmp_fd], moni, "502", Initer::initMapConfig(moni, tmp_fd)));
		}
		else
		{
			(*moni.clients[tmp_fd]).first_pass = false;
			(*moni.clients[tmp_fd]).content_real = 0;
			(*moni.clients[tmp_fd]).content_lenght = 0;

			close((*moni.clients[tmp_fd]).pipe_write[1]);
			close((*moni.clients[tmp_fd]).pipe_write[0]);
			moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
		}
	}
	else if (moni.clients[fd]->getStatusCgi() && !moni.clients[fd]->getOutput().empty())
	{
		if (moni.clients[fd]->responce_cgi)
		{
			Modifier::modifyMapCookies(moni, const_cast<std::string&>(moni.clients[fd]->getOutput()));
			ssize_t bytes_sent = send(fd, moni.clients[fd]->getOutput().c_str(), moni.clients[fd]->getOutput().size(), 0);
			if (bytes_sent == 0)
			{
				std::cerr << RED "Sending Error socket data close" << std::endl;
				delete_client(moni, fd, i);
				return;
			}
			else if (bytes_sent == -1)
			{
				std::cerr << RED "Send Error CONNECTION" << std::endl;
				delete_client(moni, fd, i);
				return;
			}
			delete_client(moni, fd, i);
		}
	}
	else if (!moni.clients[fd]->getStatusCgi())
	{
		if (!(*moni.clients[fd]).getInput().empty() && (*moni.clients[fd]).content_real >= (*moni.clients[fd]).content_lenght)
		{
			if (raph(moni, *moni.clients[fd]))
				return;
		}
		if (!(*moni.clients[fd]).getStatusCgi() && !(*moni.clients[fd]).getOutput().empty())
		{
			ssize_t bytes_sent = send((*moni.clients[fd]).getFD(), (*moni.clients[fd]).getOutput().c_str(), (*moni.clients[fd]).getOutput().size(), 0);
			if (bytes_sent == 0)
			{
				std::cerr << RED "Sending Error socket data close" << std::endl;
				delete_client(moni, fd, i);
				return;
			}
			else if(bytes_sent == -1)
			{
				std::cerr << RED "Send Error CONNECTION" << std::endl;
				delete_client(moni, fd, i);
				return;
			}
			delete_client(moni, fd, i);
		}
	}
}

void	error(monitoring &moni, pollfd &poll, int i)
{
	client *cl = moni.clients[poll.fd];
	std::map<int, client *>::iterator it = moni.clients.find(poll.fd);

	if (it != moni.clients.end() && cl)
	{
		if(Checker::compar(cl->getFD(), moni.all_pollfd_servor))
			std::cerr <<RED << "is a servor" << RESET << std::endl;
	}
	else if (poll.revents & POLLERR)
		std::cerr <<RED << "POLLERR" << RESET << std::endl;
	else if (poll.revents & POLLHUP)
		std::cerr <<RED << "POLLHUP" << RESET << std::endl;
	else if (poll.revents & POLLNVAL)
		std::cerr << RED << "POLLNVAL" << RESET << std::endl;
	else if (poll.revents & POLLRDHUP)
		std::cerr << RED << "POLLRDHUP" << RESET << std::endl;

	std::cerr << "in fd number : " << poll.fd << std::endl;
	delete_client(moni, poll.fd, i);
}

bool	raph(monitoring &moni, client &cl)
{
	/* reponce */
	RequestIn test(cl, moni);
    if (test.getCode() == 200)
        test.checkErrorHTTPHeaders();
    if (test.getCode() == 200)
        test.checkErrorHTTPBody();
    if (test.getCode() == 200){
        test.parseBody();
	}
	std::string response = test.makeResponse();

	// std::cerr << BLUE << "response : \n" << ORANGE << response << RESET << std::endl; /* remove if you want to see the output */

	if (!cl.getStatusCgi())
		cl.setOutput(response);
	else
		cl.AddInput(response);
	return cl.getStatusCgi();
}
