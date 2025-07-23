/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 17:26:52 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/23 17:44:58 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include "minirt.h"
#include "defines.h"
#include "utils.h"

bool	get_model_size(char *file_name, int *p_size, int *m_size)
{
	int		fd;
	char	*line;
	char	*pline;

	fd = open(file_name, O_RDONLY);
	*p_size = 0;
	*m_size = 0;
	pline = NULL;
	while (get_line(fd, &pline))
	{
		line = pline;
		skip_spaces(&line);
		if (ft_strncmp(line, "p ", 2) == 0)
			(*p_size)++;
		else if (ft_strncmp(line, "t ", 2) == 0)
			(*m_size)++;
		else if (*line && *line != '\n')
			return (free(pline), false);
	}
	close(fd);
	if (*p_size == 0 || *m_size == 0)
		return (false);
	return (true);
}

bool	get_point(char *data, t_vec3 *p)
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

static bool	get_next_index(char **data, int *index)
{
	*index = ft_atoi(*data);
	while (**data && **data != ',')
		(*data)++;
	if (**data == ',')
		(*data)++;
	return (true);
}

bool	get_triangle(char *data, t_tri *triangle, t_vec3 *points, int ps)
{
	int		i;

	if (!is_correct_coordinate(data))
		return (false);
	if (!get_next_index(&data, &i) || i < 0 || i >= ps)
		return (false);
	triangle->v0 = &points[i];
	if (!get_next_index(&data, &i) || i < 0 || i >= ps)
		return (false);
	triangle->v1 = &points[i];
	if (!get_next_index(&data, &i) || i < 0 || i >= ps)
		return (false);
	triangle->v2 = &points[i];
	triangle->edge1 = v_sub(*triangle->v1, *triangle->v0);
	triangle->edge2 = v_sub(*triangle->v2, *triangle->v0);
	return (true);
}
