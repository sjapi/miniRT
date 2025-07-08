/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:01:55 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/08 17:02:59 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include "parser.h"
#include "utils.h"
#include "get_next_line.h"
#include "defines.h"

bool	print_err(char *msg)
{
	printf("miniRT: parser: %s\n", msg);
	return (false);
}

bool	parse_plane(char *plane_data, t_obj *plane)
{
	plane->type = PLANE;
	skip_info(&plane_data);
	if (!get_coordinates(plane_data, &plane->center))
		return (free(plane), print_err("Plane has invalid coordinates"));
	skip_info(&plane_data);
	if (!get_orientation(plane_data, &plane->norm_vector))
		return (free(plane), print_err("Plane has invalid orientation"));
	skip_info(&plane_data);
	if (!get_color(plane_data, &plane->color))
		return (free(plane), print_err("Plane has invalid color"));
	skip_info(&plane_data);
	if (*plane_data && *plane_data != '\n')
		return (free(plane), print_err("Light has invalid data"));
	return (true);
}

bool	parse_obj(char *obj_data, t_scene *scene)
{
	t_obj	*obj;

	obj = malloc(sizeof(t_obj));
	if (!obj)
		return (print_err("Can't allocate memory"));
	if (ft_strncmp(obj_data, "pl ", 3) == 0)
	{
		if (!parse_plane(obj_data, obj))
			return (false);
	}
	else
		return (true); // TODO: unkown type
	scene->objs = obj;
	scene->objs_count++;
	return (true);
}

t_scene	*load_scene(char *file_name)
{
	t_scene	*scene;
	char	*line;
	int		fd;

	if (!is_valid_name(file_name))
		return (NULL);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (NULL);
	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	line = get_next_line(fd);
	while (line)
	{
		skip_spaces(&line);
		if (*line || *line != '\n')
		{
			if (ft_strncmp(line, "A ", 2) == 0)
			{
				if (!parse_ambient(line, scene))
					return (NULL);
			}
			else if (ft_strncmp(line, "C ", 2) == 0)
			{
				if (!parse_camera(line, scene))
					return (NULL);
			}
			else if (ft_strncmp(line, "L ", 2) == 0)
			{
				if (!parse_light(line, scene))
					return (NULL);
			}
			else
			{
				if (!parse_obj(line, scene))
					return (NULL);
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	return (scene);
}
