/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:46:55 by ouvled            #+#    #+#             */
/*   Updated: 2025/10/28 21:55:30 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <vector>
# include <string>
# include <iostream>
# include "ParseException.hpp"
# include "DirectiveTypes.hpp"
# include "Config.hpp"
# include "Token.hpp"
# include "Tokenizer.hpp"

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
	ServerConfig		_currentServer;
	LocationConfig		_currentLocation;
	Config				_config;
	std::string			_configContent;
	std::string			_configFileName;
	std::vector<Token>	_tokens;
	Tokenizer			_tokenizer;
public:
	ConfigParser(char *configFile);
	Config				parseConfig();
private:
	void				handleGlobalState(std::vector<Token>::const_iterator &it);
	void				handleServerState(std::vector<Token>::const_iterator &it);
	void				handleLocationState(std::vector<Token>::const_iterator &it);

	void				parseListenDirective(std::vector<Token>::const_iterator &it);
	void				parseServerRootDirective(std::vector<Token>::const_iterator &it);
	void				parseErrorPageDirective(std::vector<Token>::const_iterator &it);
	void				parseServerBodySizeDirective(std::vector<Token>::const_iterator &it);

	void				parseLocationRootDirective(std::vector<Token>::const_iterator &it);
	void				parseIndexDirective(std::vector<Token>::const_iterator &it);
	void				parseAllowMethodsDirective(std::vector<Token>::const_iterator &it);
	void				parseAutoIndexDirective(std::vector<Token>::const_iterator &it);
	void				parseUploadEnableDirective(std::vector<Token>::const_iterator &it);
	void				parseUploadStoreDirective(std::vector<Token>::const_iterator &it);
	void				parseReturnDirective(std::vector<Token>::const_iterator &it);
	void				parseCgiExtensionDirective(std::vector<Token>::const_iterator &it);
	void				parseLocationBodySizeDirective(std::vector<Token>::const_iterator &it);

	void				expectToken(std::vector<Token>::const_iterator &it, TokenType type, std::string errorMsg);
	size_t				convertSize(std::vector<Token>::const_iterator &it, const std::string &bodySize);
};

#endif