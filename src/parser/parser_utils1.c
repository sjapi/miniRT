/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:40:12 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/22 16:03:32 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

bool	is_float(char *num)
{
	int		digits;
	bool	has_dot;

	has_dot = false;
	if (*num == '-' || *num == '+')
		num++;
	digits = count_digits(num);
	while (*num)
	{
		if (*num == '.')
		{
			if (has_dot)
				return (false);
			has_dot = true;
			digits = count_digits(&num[1]);
		}
		else if (*num == '\n' || is_whitespace(*num))
			break ;
		else if (!ft_isdigit(*num))
			return (false);
		num++;
	}
	return (digits > 0 && digits < 11);
}

bool	is_int(char *num)
{
	int	digits;

	digits = count_digits(num);
	while (ft_isdigit(*num))
		num++;
	if ((*num && *num != '\n' && !is_whitespace(*num)))
		return (false);
	return (digits > 0 && digits < 11);
}

bool	parse_float(char **str, int *digits)
{
	if (**str == '-')
		(*str)++;
	*digits = count_digits(*str);
	while (ft_isdigit(**str))
		(*str)++;
	if (!**str || **str == '\n' || is_whitespace(**str) || **str == ',')
		return (*digits > 0 && *digits < 11);
	if (**str != '.' || *digits == 0)
		return (false);
	(*str)++;
	*digits = count_digits(*str);
	while (ft_isdigit(**str))
		(*str)++;
	if (!**str || **str == '\n' || is_whitespace(**str) || **str == ',')
		return (*digits > 0 && *digits < 11);
	return (false);
}
