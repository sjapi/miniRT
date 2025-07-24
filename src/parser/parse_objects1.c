/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:08:03 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/24 15:11:17 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include "parser.h"
#include "defines.h"
#include "utils.h"

static bool	parse_texture(char **obj_data, t_obj *obj)
{
	char	*file_name;

	if (!**obj_data || **obj_data == '\n')
		return (true);
	if (is_checkerboard(*obj_data))
		obj->checkerboard = true;
	else if (is_mirror(*obj_data))
		obj->mirror = true;
	else if (get_file_name(*obj_data, &file_name))
	{
		if (!is_valid_file(file_name, ".xpm"))
			return (free(file_name), print_err("Invalid texture"));
		obj->texture = ft_calloc(sizeof(t_texture), 1);
		if (!obj->texture)
			return (print_err("Can't allocate memory"));
		obj->texture->file_name = file_name;
	}
	else
		return (print_err("Invalid file name"));
	next_info(obj_data);
	return (true);
}

bool	parse_plane(char *plane_data, t_obj *plane)
{
	plane->type = PLANE;
	next_info(&plane_data);
	if (!get_coordinates(plane_data, &plane->center))
		return (print_err("Plane has invalid coordinates"));
	next_info(&plane_data);
	if (!get_orientation(plane_data, &plane->norm_vector))
		return (print_err("Plane has invalid orientation"));
	next_info(&plane_data);
	if (!get_color(plane_data, &plane->color))
		return (print_err("Plane has invalid color"));
	next_info(&plane_data);
	parse_texture(&plane_data, plane);
	if (*plane_data && *plane_data != '\n')
		return (print_err("Plane has invalid data"));
	return (true);
}

bool	parse_sphere(char *sphere_data, t_obj *sphere)
{
	sphere->type = SPHERE;
	sphere->attrs = malloc(sizeof(float) * SPHERE_ATTR_COUNT);
	if (!sphere->attrs)
		return (print_err("Can't allocate memory"));
	next_info(&sphere_data);
	if (!get_coordinates(sphere_data, &sphere->center))
		return (print_err("Sphere has invalid coordinates"));
	next_info(&sphere_data);
	if (!get_attribute(sphere_data, &sphere->attrs[SPHERE_D_I]))
		return (print_err("Sphere has invalid diameter"));
	next_info(&sphere_data);
	if (!get_color(sphere_data, &sphere->color))
		return (print_err("Sphere has invalid color"));
	if (next_info(&sphere_data) && !parse_texture(&sphere_data, sphere))
		return (print_err("Sphere has invalid texture"));
	if (*sphere_data && *sphere_data != '\n')
		return (print_err("Sphere has invalid data"));
	return (true);
}

bool	parse_cylinder(char *data, t_obj *cylinder)
{
	cylinder->type = CYLINDER;
	cylinder->attrs = malloc(sizeof(float) * CYLINDER_ATTR_COUNT);
	if (!cylinder->attrs)
		return (print_err("Can't allocate memory"));
	if (next_info(&data) && !get_coordinates(data, &cylinder->center))
		return (print_err("Cylinder has invalid coordinates"));
	if (next_info(&data) && !get_orientation(data, &cylinder->norm_vector))
		return (print_err("Cylinder has invalid orientation"));
	if (next_info(&data) && !get_attribute(data, &cylinder->attrs[0]))
		return (print_err("Cylinder has invalid diameter"));
	if (next_info(&data) && !get_attribute(data, &cylinder->attrs[1]))
		return (print_err("Cylinder has invalid height"));
	if (next_info(&data) && !get_color(data, &cylinder->color))
		return (print_err("Cylinder has invalid color"));
	if (next_info(&data) && !parse_texture(&data, cylinder))
		return (print_err("Cylinder has invalid texture"));
	if (next_info(&data) && *data && *data != '\n')
		return (print_err("Cylinder has invalid data"));
	return (true);
}

/*
 * co     0,0,0   0,1,0   30         5      255,0,0
 *        center  norm    half_angle height color
 */
bool	parse_cone(char *data, t_obj *cone)
{
	cone->type = CONE;
	cone->attrs = malloc(sizeof(float) * CONE_ATTR_COUNT);
	if (!cone->attrs)
		return (print_err("Can't allocate memory"));
	if (next_info(&data) && !get_coordinates(data, &cone->center))
		return (print_err("Cone has invalid coordinates"));
	if (next_info(&data) && !get_orientation(data, &cone->norm_vector))
		return (print_err("Cone has invalid orientation"));
	if (next_info(&data) && !get_attribute(data, &cone->attrs[CONE_A_I]))
		return (print_err("Cone has invalid angle"));
	if (next_info(&data) && !get_attribute(data, &cone->attrs[CONE_H_I]))
		return (print_err("Cone has invalid height"));
	if (next_info(&data) && !get_color(data, &cone->color))
		return (print_err("Cone has invalid color"));
	if (next_info(&data) && !parse_texture(&data, cone))
		return (print_err("Con has invalid texute"));
	if (next_info(&data) && *data && *data != '\n')
		return (print_err("Cone has invalid data"));
	return (true);
}
