#include <unistd.h>
#include "utils.h"
#include "parser.h"

bool	is_valid_name(char *file_name)
{
	int	size;

	size = ft_strlen(file_name);
	if (size < 3)
		return (false);
	if (ft_strcmp(&file_name[size - 3], ".rt") != 0)
		return (false);
	return (true);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

bool	is_float(char *num)
{
	while (*num >= '0' && *num <= '9')
		num++;
	if (*num == '.')
	{
		while (*num >= '0' && *num <= '9')
			num++;
	}
	if (*num && *num != '\n' && !is_whitespace(*num))
		return (false);
	return (true);
}

bool	is_int(char *num)
{
	while (*num >= '0' && *num <= '9')
		num++;
	if (*num && *num != '\n' && !is_whitespace(*num))
		return (false);
	return (true);
}

bool	is_correct_color(char *str)
{
	int	commas;
	int	digits;

	commas = 0;
	skip_spaces(&str);
	while (*str)
	{
		if (skip_integer(&str, &digits)	== 0 || digits > 3)
			return (false);
		if (!*str || *str == '\n')
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

bool is_correct_coordinate(char *str)
{
	int	commas;
	int	digits;

	commas = 0;
	skip_spaces(&str);
	while (str)
	{
		if (!parse_float(&str, &digits))
		    return (false);
		if (!*str || *str == '\n')
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
// ================================================

void	skip_spaces(char **str)
{
    while (is_whitespace(**str))
        (*str)++;
}

bool	skip_integer(char **str, int *digits)
{
	*digits = 0;
	while (**str >= '0' && **str <= '9')
	{
		(*digits)++;
		(*str)++;
	}
	return (*digits > 0);
}

void	skip_info(char **data)
{
	while (data && !is_whitespace(**data))
		(*data)++;
}

// ===============================================
bool parse_float(char **str, int *digits)
{
	*digits = 0;
	while (**str >= '0' && **str <= '9')
	{
		(*digits)++;
		(*str)++;
	}
	if (!**str || **str == '\n' || is_whitespace(**str))
		return (*digits > 0);
	if (**str != '.' || *digits == 0)
		return (false);
	(*str)++;
	*digits = 0;
	while (**str >= '0' && **str <= '9')
	{
	    (*digits)++;
	    (*str)++;
	}
	return (*digits > 0);
}

