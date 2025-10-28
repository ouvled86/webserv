/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouvled <ouvled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:31:14 by ouel-bou          #+#    #+#             */
/*   Updated: 2025/10/28 21:20:38 by ouvled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: webserv <config_file>" << std::endl;
		return 1;
	}
	try
	{
		ConfigParser	parser = ConfigParser(argv[1]);
		Config			servConfig = parser.parseConfig();
		(void)servConfig;
	}
	catch (const ParseException &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (const std::exception &e)
	{
		std::cerr << "webserv: " << e.what() << std::endl;
		return 1;
	}
}
