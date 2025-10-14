/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:47:00 by ouvled            #+#    #+#             */
/*   Updated: 2025/10/14 01:03:34 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ConfigParser.hpp"

ConfigParser::ConfigParser(char *conf_file) : _pState(GLOBAL_STATE)
{
	_tokenizer = Tokenizer(conf_file);
	_config = _tokenizer.getConfig();
	Token	holder;
	while ((holder = _tokenizer.getNextToken()).getType() != T_EOF)
	{
		_tokens.push_back(holder);
		std::cout << "Token value is: " << holder.getContent() << " of type: " << holder.getType() << " at line: " << holder.getLine() << ", posInLine: " << holder.getPosInLine() << std::endl;
	}
}

ConfigParser::~ConfigParser()
{
	
}

// Config		ConfigParser::parseConfig()
// {
	
// }
