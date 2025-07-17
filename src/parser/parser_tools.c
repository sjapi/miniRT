/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:05:46 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/17 22:03:47 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include "minirt.h"
#include "parser.h"
#include "utils.h"

bool	get_file_name(char *str, char **name)
{
	int		size;

	size = 0;
	while (str[size] && !is_whitespace(str[size]) && str[size] != '\n')
		size++;
	if (size == 0)
		return (false);
	*name = malloc(sizeof(char) * (size + 1));
	if (!*name)
		return (print_err("Can't allocate memory"));
	ft_memcpy(*name, str, size);
	(*name)[size] = '\0';
	return (true);
}

bool	get_color(char *obj_data, int *color_code)
{
	int	n;

	if (!is_correct_color(obj_data))
		return (false);
	*color_code = 0;
	n = 0;
	while (*obj_data >= '0' && *obj_data <= '9')
		n = n * 10 + (*obj_data++ - '0');
	*color_code = n;
	n = 0;
	obj_data++;
	while (*obj_data >= '0' && *obj_data <= '9')
		n = n * 10 + (*obj_data++ - '0');
	*color_code = (*color_code << 8) | n;
	n = 0;
	obj_data++;
	while (*obj_data >= '0' && *obj_data <= '9')
		n = n * 10 + (*obj_data++ - '0');
	*color_code = (*color_code << 8) | n;
	return (true);
}

bool	get_coordinates(char *data, t_vec3 *p)
{
	if (!is_correct_coordinate(data))
		return (false);
	p->x = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	p->y = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	p->z = ft_atof(data);
	return (true);
}

bool	get_orientation(char *data, t_vec3 *o)
{
	if (!is_correct_coordinate(data))
		return (false);
	o->x = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	o->y = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	o->z = ft_atof(data);
	if (!v_normalized(*o))
		return (false);
	return (true);
}

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
