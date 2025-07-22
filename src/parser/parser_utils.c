/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:03:20 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/22 15:45:26 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "utils.h"
#include "parser.h"

bool	is_valid_file(char *file_name, char *ext)
{
	int	size;
	int	ext_size;
	int	fd;

	ext_size = ft_strlen(ext);
	size = ft_strlen(file_name);
	if (size < ext_size)
		return (false);
	if (ft_strcmp(&file_name[size - ext_size], ext) != 0)
		return (false);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (false);
	close(fd);
	return (true);
}

bool	is_correct_color(char *str)
{
	int	commas;
	int	digits;

	commas = 0;
	while (*str)
	{
		if (skip_integer(&str, &digits) == 0 || digits > 3)
			return (false);
		if (!*str || *str == '\n' || is_whitespace(*str))
			break ;
		if (*str == ',')
			commas++;
		else if (!is_whitespace(*str))
			return (false);
		str++;
	}
	return (commas == 2);
}

bool	is_correct_coordinate(char *str)
{
	int	commas;
	int	digits;

	commas = 0;
	while (str)
	{
		if (!parse_float(&str, &digits))
			return (false);
		if (!*str || *str == '\n' || is_whitespace(*str))
			break ;
		if (*str == ',')
		{
			commas++;
			str++;
		}
		else if (*str < 0 && *str > 9)
			return (false);
	}
	return (commas == 2);
}

bool	is_checkerboard(char *str)
{
	if (ft_strncmp("checkerboard", str, 12) != 0)
		return (false);
	if (str[12] && str[12] != '\n' && !is_whitespace(str[12]))
		return (false);
	return (true);
}

bool	is_mirror(char *str)
{
	if (ft_strncmp("mirror", str, 6) != 0)
		return (false);
	if (str[6] && str[6] != '\n' && !is_whitespace(str[6]))
		return (false);
	return (true);
}
