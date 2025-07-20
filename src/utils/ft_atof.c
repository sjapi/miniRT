/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:12:06 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/20 17:43:57 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	i_love_norminette(float *r, float *f, float *d, int *s)
{
	*r = 0.0f;
	*f = 0.0f;
	*d = 1.0f;
	*s = 1;
}

static void	set_sign(char **str, int *sign)
{
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

float	ft_atof(char *str)
{
	float	result;
	float	fraction;
	float	divisor;
	int		sign;

	i_love_norminette(&result, &fraction, &divisor, &sign);
	set_sign(&str, &sign);
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10.0f + (*str - '0');
		str++;
	}
	if (*str == '.')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		fraction = fraction * 10.0f + (*str - '0');
		divisor *= 10.0f;
		str++;
	}
	result += fraction / divisor;
	return (result * sign);
}
