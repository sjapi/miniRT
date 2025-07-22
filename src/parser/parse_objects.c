/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:21:58 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/22 16:08:55 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include "parser.h"
#include "defines.h"
#include "utils.h"
#include "bounding.h"

static void	free_obj(t_obj *obj)
{
	if (obj->attrs)
		free(obj->attrs);
	if (obj->mesh)
	{
		free(obj->mesh->points);
		free(obj->mesh->triangles);
		free(obj->mesh->file_name);
		free(obj->mesh);
	}
	free(obj);
}

static bool	try_parse_object(char *data, t_obj *obj)
{
	return ((ft_strncmp(data, "pl ", 3) == 0 && parse_plane(data, obj))
		|| (ft_strncmp(data, "sp ", 3) == 0 && parse_sphere(data, obj))
		|| (ft_strncmp(data, "cy ", 3) == 0 && parse_cylinder(data, obj))
		|| (ft_strncmp(data, "co ", 3) == 0 && parse_cone(data, obj))
		|| (ft_strncmp(data, "obj ", 4) == 0 && parse_model(data, obj)));
}

bool	parse_obj(char *obj_data, t_scene *scene)
{
	t_obj	*obj;

	obj = ft_calloc(1, sizeof(t_obj));
	if (!obj)
		return (print_err("Can't allocate memory"));
	if (try_parse_object(obj_data, obj))
	{
		calculate_aabb(obj);
		append_obj(scene, obj);
		return (true);
	}
	free_obj(obj);
	return (print_err(NULL));
}
