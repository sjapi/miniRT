#include <unistd.h>
#include "utils.h"

int	is_valid_name(char *file_name)
{
	int	size;

	size = ft_strlen(file_name);
	if (size < 3)
		return (0);
	if (ft_strcmp(&file_name[size - 3], ".rt") != 0)
		return (0);
	return (1);
}

int	is_white_space(char c)
{
	return (c == ' ' || c == '\t');
}

char    *trim_spaces(char *str)
{
    int size;

    while (*str && is_white_space(*str))
        str++;
    return (str);
}

bool	is_correct_color(char *str)
{
	int	comas;

	comas = 0;
	while (*str && *str >= '0' && *str <= '9')
	{
		str++;
		if (*str == ',')
		{
			comas++;
			str++;
			if (str[1] || str[1] != ',' || str[1] != '\n')
				return (false);
		}
	}
	if (comas != 2)
		return (false);
	return (true);
}
