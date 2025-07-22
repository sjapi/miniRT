/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perser_tools1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:04:16 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/22 16:05:12 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

bool	get_ratio(char *data, float *ratio)
{
	if (!is_float(data))
		return (false);
	*ratio = ft_atof(data);
	if (*ratio < 0.0 || *ratio > 1.0)
		return (false);
	return (true);
}

bool	get_fov(char *data, int *fov)
{
	if (!is_int(data))
		return (false);
	*fov = ft_atoi(data);
	if (*fov < 0 || *fov > 180)
		return (false);
	return (true);
}

bool	get_attribute(char *data, float *attr)
{
	if (!is_float(data))
		return (false);
	*attr = ft_atof(data);
	if (*attr < 0.0)
		return (false);
	return (true);
}
