/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:26:41 by ouvled            #+#    #+#             */
/*   Updated: 2025/10/14 01:03:43 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Token.hpp"

Token::Token()
{

}

Token::Token(std::string content, TokenType type, size_t posInLine, int line) : _content(content), _type(type), _posInLine(posInLine), _line(line)
{
	
}

const std::string	&Token::getContent() const
{
	return _content;
}

TokenType	Token::getType() const
{
	return _type;
}

size_t		Token::getPosInLine() const
{
	return _posInLine;
}

int			Token::getLine() const
{
	return _line;
}
