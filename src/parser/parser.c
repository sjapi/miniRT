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

bool	append_obj(t_scene *scene, t_obj *obj)
{
	t_obj	*objs;
	int	i;

	objs = malloc(sizeof(t_obj) * (scene->objs_count + 1));
	if (!objs)
		return (false);
	i = 0;
	while (i < scene->objs_count)
	{
		objs[i] = scene->objs[i];
		objs[i].attrs = scene->objs[i].attrs;
		i++;
	}
	objs[i] = *obj;
	objs[i].attrs = obj->attrs;
	free(scene->objs);
	scene->objs = objs;
	scene->objs_count++;
	return (true);
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
		return (free(plane), print_err("Plane has invalid data"));
	return (true);
}

bool	parse_sphere(char *sphere_data, t_obj *sphere)
{
	sphere->type = SPHERE;
	sphere->attrs = malloc(sizeof(float));
	if (!sphere->attrs)
		return (free(sphere), print_err("Can't allocate memory"));	
	skip_info(&sphere_data);
	if (!get_coordinates(sphere_data, &sphere->center))
		return (free(sphere->attrs),
			free(sphere), print_err("Sphere has invalid coordinates"));
	skip_info(&sphere_data);
	if (!get_diameter(sphere_data, &sphere->attrs[SPHERE_D_I]))
		return (free(sphere->attrs),
			free(sphere), print_err("Sphere has invalid diameter"));
	skip_info(&sphere_data);
	if (!get_color(sphere_data, &sphere->color))
		return (free(sphere->attrs),
			free(sphere), print_err("Sphere has invalid color"));
	skip_info(&sphere_data);
	if (*sphere_data && *sphere_data != '\n')
		return (free(sphere->attrs),
			free(sphere), print_err("Sphere has invalid data"));
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
	else if (ft_strncmp(obj_data, "sp ", 3) == 0)
	{
		if (!parse_sphere(obj_data, obj))
			return (false);
	}
	else
		return (true); // TODO: unkown type
	return (append_obj(scene, obj));
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
