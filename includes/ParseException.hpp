/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseException.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 23:30:41 by ouvled            #+#    #+#             */
/*   Updated: 2025/10/14 01:04:59 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEEXCEPTION_HPP
# define PARSEEXCEPTION_HPP

#include <sstream>

class ParseException : public std::exception
{
private:
	std::string	_errMsg;
	std::string	_fileName;
	size_t		_posInLine;
	int			_line;
public:
	ParseException(const std::string &errMsg, const std::string &fileName, size_t posInLine, int line)
		: _errMsg(errMsg), _fileName(fileName), _posInLine(posInLine), _line(line) {}
	virtual ~ParseException() throw() {}
	virtual const char	*what() const throw()
	{
		static std::string	fullMessage;
		std::ostringstream	oss;
		if (_line > 0)
			oss << "webserv: " << _errMsg << " in file: " << _fileName << ":" << _line << ":" << _posInLine;
		else
			oss << "webserv: " << _errMsg << " in file: " << _fileName;
		fullMessage = oss.str();
		return fullMessage.c_str();
	}
};

#endif