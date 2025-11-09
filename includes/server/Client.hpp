/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 19:48:30 by ouvled            #+#    #+#             */
/*   Updated: 2025/11/09 02:48:14 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <ctime>
# include "ListeningSocket.hpp"

typedef struct	Client
{
	int							fd;
	std::pair<std::string, int>	addr;
	ListeningSocket				&acceptingSock;
	std::string					requestBuffer;
	std::string					responseBuffer;
	size_t						contentLength;
	size_t						bytesReceived;
	size_t						bytesSent;
	bool						reqParsed;
	bool						resSent;
	time_t						timeout;
	Client(int clientFd, std::string clientAddr, int clientPort, ListeningSocket &acceptingSocket);
}				Client;


#endif