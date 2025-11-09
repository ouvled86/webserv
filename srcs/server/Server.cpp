/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 19:54:49 by ouvled            #+#    #+#             */
/*   Updated: 2025/11/10 00:39:15 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/Server.hpp"

Server::Server(Config &config, std::vector<std::pair<int, std::pair<std::string, int> > > allListens, int serverCount) : _conf(config), _allListens(allListens), _serverCount(serverCount)
{
	openListeningSocks();
}

void	Server::startServer()
{
	while (86)
	{
		std::vector<struct pollfd>	polls;
		for (size_t i = 0; i < _listeningSocketsCount; i++)
		{
			struct pollfd	pfd;
			pfd.fd = _socks[i]._fd;
			pfd.events = POLLIN;
			pfd.revents = 0;
			polls.push_back(pfd);
		}
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			struct pollfd	pfd;
			pfd.fd = it->first;
			if (!it->second.resSent)
			{
				if (!it->second.responseBuffer.empty())
					pfd.events = POLLOUT;
				else
					pfd.events = POLLIN;
			}
			else
				pfd.events = POLLIN;
			pfd.revents = 0;
			polls.push_back(pfd);
		}
		int	pRet = poll(polls.data(), static_cast<nfds_t>(polls.size()), 1000);
		if (pRet < 0)
		{
			if (errno == EINTR)
				continue ;
			throw std::runtime_error("webserv: poll() failure in the main event loop");
		}
		timeoutChecker();
		if (!pRet)
			continue ;
		for (size_t i = 0; i < polls.size(); i++)
		{
			if (!polls[i].revents)
				continue ;
			if (i < _listeningSocketsCount && (polls[i].revents & POLLIN))
				acceptNewClient(_socks[i]);
			else if (i >= _listeningSocketsCount)
			{
				int	clientFd = polls[i].fd;
				if ((polls[i].revents & POLLERR) || (polls[i].revents & POLLHUP ) || (polls[i].revents & POLLNVAL))
				{
					deleteClient(clientFd);
					continue ;
				}
				// read from client bitch ass monkee
				if (polls[i].revents & POLLIN)
					handleClientInput(_clients.find(clientFd)->second);
				// ready to write without blocking u faggot
				if (polls[i].revents & POLLOUT)
					handleClientOutput(_clients.find(clientFd)->second);
			}
		}
	}
}

void	Server::openListeningSocks()
{
	std::vector<ServerConfig>::iterator										it = _conf.configs.begin();
	std::vector<std::pair<int, std::pair<std::string, int> > >::iterator	it2 = _allListens.begin();
	for (int i = 1; i <= _serverCount; i++)
	{
		while (it2 != _allListens.end() && it2->first == i)
		{
			int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0)
				throw std::runtime_error("webserv: Failed to create socket");
			handleSock(sockfd, it2->second, T_SERVER);
			_socks.push_back(ListeningSocket(*it, it2->second.first, it2->second.second, sockfd));
			++it2;
		}
		if (i != _serverCount)
			++it;
	}
	_listeningSocketsCount = _socks.size();
	std::cout << "Opened all listening sockets successfully" << std::endl;
}

void	Server::handleSock(int sockfd, std::pair<std::string, int> bindInfo, E_SOCKTYPE type)
{
	if (type == T_SERVER)
	{
		if (setRUASock(sockfd) < 0)
			throw std::runtime_error("webserv: Failed to set socket SO_REUSEADDR option");
		if (setNBSock(sockfd) < 0)
			throw std::runtime_error("webserv: Failed to set file descriptor non-blocking");
		bindAndListen(sockfd, bindInfo);
	}
}

int	Server::setRUASock(int sockfd)
{
	int	opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return (-1);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
		return (-1);
	return (0);
}

int	Server::setNBSock(int sockfd)
{
	int	flags = fcntl(sockfd, F_GETFL, 0);
	if (flags < 0)
		return (-1);
	if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
		return (-1);
	return (0);
}

void	Server::bindAndListen(int sockfd, std::pair<std::string, int> bindInfo)
{
	struct sockaddr_in	addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(bindInfo.second);
	if (bindInfo.first == "0.0.0.0")
		addr.sin_addr.s_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, bindInfo.first.c_str(), &addr.sin_addr);
	if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
		throw std::runtime_error("webserv: Failed to bind socket");
	if (listen(sockfd, SOMAXCONN) < 0)
		throw std::runtime_error("webserv: listen() failed");
}

void	Server::timeoutChecker()
{
	time_t				currentTime;
	const time_t		timeout = 60;
	std::vector<int>	toClose;

	currentTime = time(NULL);
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (currentTime - it->second.timeout > timeout)
		{
			std::cout << "Client timeout, socket fd: " << it->first << std::endl;
			toClose.push_back(it->first);
		}
	}
	for (size_t i = 0; i < toClose.size(); i++)
		deleteClient(toClose[i]);
}

void	Server::acceptNewClient(ListeningSocket &acceptingSocket)
{
	socklen_t			clientAddrLen;
	struct sockaddr_in	clientAddr;
	int					clientFd;
	char				address[INET_ADDRSTRLEN];
	int					clientPort;

	clientAddrLen = sizeof(clientAddr);
	clientFd = accept(acceptingSocket._fd, reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);
	if (clientFd < 0)
	{
		if (errno == EWOULDBLOCK || errno == EAGAIN)
			return ;
		std::cerr << "webserv: accept() failed to accept new client\n" << "webserv: " << strerror(errno) << std::endl;
		return ;
	}
	if (setNBSock(clientFd) < 0)
	{
		std::cerr << "webserv: Failed to set client's file descriptor non-blocking" << std::endl;
		return ;
	}
	inet_ntop(AF_INET, reinterpret_cast<const void *>(&clientAddr.sin_addr.s_addr), address, INET_ADDRSTRLEN);
	clientPort = ntohs(clientAddr.sin_port);
	Client				newClient = Client(clientFd, address, clientPort, acceptingSocket);
	_clients.insert(std::make_pair(clientFd, newClient));
	std::cout << "_clients size is: " << _clients.size() << " Accepted new client of ip addr: " << _clients.find(clientFd)->second.addr.first << " and port: " << _clients.find(clientFd)->second.addr.second << " successfully!" << std::endl;
}

void	Server::deleteClient(int clientFd)
{
	_clients.erase(clientFd);
	close(clientFd);
	std::cout << "deleting client fd numba: " << clientFd << std::endl;
}

void	Server::handleClientInput(Client &client)
{
	char	clientRecvBuffer[1024];
	ssize_t	ret;

	ret = recv(client.fd, clientRecvBuffer, sizeof(clientRecvBuffer) - 1, 0);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		std::cerr << "webserv: recv() syscall failed\n webserv: " << strerror(errno) << std::endl;
		deleteClient(client.fd);
		return ;
	}
	if (!ret)
	{
		deleteClient(client.fd);
		return ;
	}
	client.timeout = time(NULL);
	clientRecvBuffer[ret] = '\0';
	client.requestBuffer.append(clientRecvBuffer);
	client.bytesReceived += ret;
	if (!client.reqParsed)
	{
		size_t	headerEnd = client.requestBuffer.find("\r\n\r\n");
		if (headerEnd != std::string::npos)
			client.reqParsed = true;
	}
	if (client.reqParsed)
		std::cout << client.requestBuffer << std::endl;
}

void	Server::handleClientOutput(Client &client)
{
	(void)client;
}
