/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:43:59 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/22 15:48:46 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "utils.h"

int	count_digits(char *num)
{
	bool	is_num;
	int		count;

	is_num = false;
	count = 0;
	while (*num == '0')
	{
		num++;
		is_num = true;
	}
	while (ft_isdigit(*num))
	{
		count++;
		num++;
	}
	if (count == 0 && is_num)
		return (1);
	return (count);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

void	skip_spaces(char **str)
{
	while (is_whitespace(**str))
		(*str)++;
}

bool	skip_integer(char **str, int *digits)
{
	*digits = count_digits(*str);
	while (**str >= '0' && **str <= '9')
		(*str)++;
	return (*digits > 0 && *digits < 11);
}

bool	next_info(char **str)
{
	while (**str && !is_whitespace(**str))
		(*str)++;
	skip_spaces(str);
	return (true);
}
