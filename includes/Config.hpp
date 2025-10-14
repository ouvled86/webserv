/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:03:55 by ouvled            #+#    #+#             */
/*   Updated: 2025/10/14 00:57:46 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

typedef struct	LocationConfig
{
	std::string					path;
	std::string					root;
	std::string					uploadTo;
	std::string					redirectTo;
	std::vector<std::string>	allowedMethods;
	std::vector<std::string>	indexes;
	bool						uploadPerm;
	bool						autoIndex;
	int							redirectionCode;
	size_t						clientMaxBodySize;
}				LocationConfig;

// This struct is assigned by values read from the config file
// listens vector contains addresses and ports that the server should be binded to for incoming connections

typedef struct	ServerConfig
{
	std::vector<std::pair<std::string, int> >	listens;
	std::vector<LocationConfig>					locations;
	std::map<int, std::string>					errorPages;
	LocationConfig								defaultLocation;
	size_t										clientMaxBodySize;
}				ServerConfig;

typedef struct	Config
{
	std::vector<ServerConfig>	configs;
}				Config;

#endif