/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:46:55 by ouvled            #+#    #+#             */
/*   Updated: 2025/10/14 01:05:31 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include "webserv.hpp"

// This enum is used as a type to determine present state during the config parsing

enum	ParsingState
{
	GLOBAL_STATE,
	SERVER_STATE,
	LOCATION_STATE,
};

// Config Parsing struct, takes av[1] then populates Config's struct with data
// Following Tokenizing/State-Machine parsing strategy

class ConfigParser
{
private:
	ParsingState		_pState;
	ServerConfig		_currentConfig;
	LocationConfig		_currentLocation;
	std::string			_config;
	std::vector<Token>	_tokens;
	Tokenizer			_tokenizer;
public:
	ConfigParser(char *conf_file);
	~ConfigParser();
	// Config				parseConfig();
};

// Helper functions
// readConfig() reads entire config file into _config
// tokenizer() breaks _config into small tokens assigned to _tokens, struct defined in Token.hpp

std::string				readConfig(std::ifstream &file);
void					skipWhiteSpaces(std::string::iterator &it);


#endif