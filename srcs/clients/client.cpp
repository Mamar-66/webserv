/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:27:35 by omfelk            #+#    #+#             */
/*   Updated: 2025/03/02 14:34:48 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/client.hpp"
#include "../../includes/Parser.hpp"

/* -------------------------------------------------------- */
/* --------------CONSTRUCTOR / DESTRUCTOR------------------ */
/* -------------------------------------------------------- */

int		creat_socket_cgi()
{
	struct sockaddr_in 		serverAddr;
	struct sockaddr_in		addr;

	int 					return_socket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = 0;

	if (bind(return_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		throw std::runtime_error("error constructeur bind cgi");

	if (listen(return_socket, 5) == -1)
		throw std::runtime_error("error constructeur listen cgi");

	/* recup port choisi par le systeme */
	socklen_t addr_len = sizeof(addr);
	if (getsockname(return_socket, (struct sockaddr *)&addr, &addr_len) == -1)
		throw std::runtime_error("Error get port");
	unsigned short	port = ntohs(addr.sin_port);

	std::cout << "CGI Port : " << port << " fd : " << return_socket << std::endl;

	return return_socket;
}

client::client(int fdsocket, int fdFather) :
socket_fd(fdsocket), socket_fd_father(fdFather),
is_cgi(false), content_lenght(0), chunked(false), first_pass(false),
responce_cgi(false), client_close(0), client_close_cgi(0), content_real(0) 
{
	this->timeout = false;
	this->startTime = std::time(NULL);
	if (this->startTime == -1)
		throw std::runtime_error(RED "Failed to get the current time" RESET);

	this->envp = NULL;

	this->clien_pollfd.fd = this->socket_fd;
	this->clien_pollfd.events = POLLIN | POLLOUT;
	this->clien_pollfd.revents = 0;

	std::cout << GREEN "creat client fd : " << this->socket_fd << "time = " << this->startTime << RESET << std::endl;
}

client::~client()
{
	if (this->socket_fd != -1)
	{
		close(this->socket_fd);
		std::cout << RED "delete client whith close fd : " << this->socket_fd << RESET << std::endl;

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
		std::cout << RED "delete client warning don't close fd : " << this->socket_fd << RESET << std::endl;
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
		// std::cout << ORANGE "creat client fd : " << tmp_fd_client << " from serveur : " << fd <<  RESET << std::endl;

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
		// std::cout << "client cgi ok write fd " << fd << std::endl;
		int tmp_fd = moni.where_are_fd_pipe(fd);
		if (tmp_fd == -1)
			std::cerr << RED "error tmp_fd where_are_fd_pipe" RESET << std::endl;

		std::cout << "tmp : " << tmp_fd << "input : " << (*moni.clients[tmp_fd]).getInput() << std::endl;

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
			std::cout << RED "write ok dans pipe" RESET << std::endl;

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
			std::cout << "cgie repondu" << std::endl;
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
			// std::cout << ORANGE "Input from responding fd " << (*moni.clients[fd]).getFD() << "= " << BLUE << (*moni.clients[fd]).getInput() << RESET << std::endl;

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
			std::cout <<RED << "is a servor" << RESET << std::endl;
	}
	else if (poll.revents & POLLERR)
		std::cout <<RED << "POLLERR" << RESET << std::endl;
	else if (poll.revents & POLLHUP)
		std::cout <<RED << "POLLHUP" << RESET << std::endl;
	else if (poll.revents & POLLNVAL)
		std::cout << RED << "POLLNVAL" << RESET << std::endl;
	else if (poll.revents & POLLRDHUP)
		std::cout << RED << "POLLRDHUP" << RESET << std::endl;

	std::cout << "dans error fd : " << poll.fd << std::endl;
	delete_client(moni, poll.fd, i);
}

bool	raph(monitoring &moni, client &cl)
{
	/* reponce */
	RequestIn test(cl, moni);
    // std::cout << test.getCode() << std::endl;
    // std::cout << "---------------" << std::endl;
    if (test.getCode() == 200) {
        test.checkErrorHTTPHeaders();
        // std::cout << test.getCode() << std::endl;
    }
    // std::cout << test.getCode() << std::endl;
    if (test.getCode() == 200) {
        test.checkErrorHTTPBody();
        // std::cout << test.getCode() << std::endl;
    }
    if (test.getCode() == 200) {
        test.parseBody();
        // std::cout << test.getCode() << std::endl;
    }
	std::string response = test.makeResponse();

	// std::cout << ORANGE << "responce : " << response << RESET << std::endl;

	if (!cl.getStatusCgi())
		cl.setOutput(response);
	else
		cl.AddInput(response);

	return cl.getStatusCgi();
}
