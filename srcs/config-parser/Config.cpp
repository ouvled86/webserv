/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 08:28:45 by ouvled            #+#    #+#             */
/*   Updated: 2025/11/03 01:18:24 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Config.hpp"

LocationConfig::LocationConfig() : path(""), root(""), uploadTo(""), redirectTo(""), uploadPerm(false), autoIndex(false), redirectionCode(0), clientMaxBodySize(0)
{
	allowedMethods.push_back("GET");
	allowedMethods.push_back("POST");
	allowedMethods.push_back("DELETE");
	indexes.push_back("index.html");
	indexes.push_back("index.htm");
}

ServerConfig::ServerConfig() : root(getenv("PWD")), clientMaxBodySize(1048576)
{
	errorPages[400] = "/error_pages/400.html";
	errorPages[403] = "/error_pages/403.html";
	errorPages[404] = "/error_pages/404.html";
	errorPages[405] = "/error_pages/405.html";
	errorPages[500] = "/error_pages/50x.html";
	errorPages[502] = "/error_pages/50x.html";
	errorPages[503] = "/error_pages/50x.html";
	errorPages[504] = "/error_pages/50x.html";
}

bool	LocationConfig::hasRedirection() const
{
	return (redirectionCode != 0);
}

bool	LocationConfig::isMethodAllowed(const std::string &method) const
{
	for (size_t i = 0; i < allowedMethods.size(); i++)
	{
		if (method == allowedMethods[i])
			return (true);
	}
	return (false);
}

size_t	LocationConfig::getBodySize(size_t serverBodySize) const
{
	if (clientMaxBodySize != 0)
		return (clientMaxBodySize);
	return (serverBodySize);
}

std::string	LocationConfig::getRoot(const std::string &serverRoot) const
{
	if (!root.empty())
		return (root);
	return (serverRoot);
}
