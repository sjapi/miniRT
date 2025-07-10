/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:21:58 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/10 21:05:34 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "defines.h"
#include "utils.h"

bool	parse_plane(char *plane_data, t_obj *plane)
{
	plane->type = PLANE;
	skip_info(&plane_data);
	if (!get_coordinates(plane_data, &plane->center))
		return (free_obj(plane), print_err("Plane has invalid coordinates"));
	skip_info(&plane_data);
	if (!get_orientation(plane_data, &plane->norm_vector))
		return (free_obj(plane), print_err("Plane has invalid orientation"));
	skip_info(&plane_data);
	if (!get_color(plane_data, &plane->color))
		return (free_obj(plane), print_err("Plane has invalid color"));
	skip_info(&plane_data);
	if (*plane_data && *plane_data != '\n')
		return (free_obj(plane), print_err("Plane has invalid data"));
	return (true);
}

bool	parse_sphere(char *sphere_data, t_obj *sphere)
{
	sphere->type = SPHERE;
	sphere->attrs = malloc(sizeof(float));
	if (!sphere->attrs)
		return (free_obj(sphere), print_err("Can't allocate memory"));	
	skip_info(&sphere_data);
	if (!get_coordinates(sphere_data, &sphere->center))
		return (free_obj(sphere), print_err("Sphere has invalid coordinates"));
	skip_info(&sphere_data);
	if (!get_attribute(sphere_data, &sphere->attrs[SPHERE_D_I]))
		return (free_obj(sphere), print_err("Sphere has invalid diameter"));
	skip_info(&sphere_data);
	if (!get_color(sphere_data, &sphere->color))
		return (free_obj(sphere), print_err("Sphere has invalid color"));
	skip_info(&sphere_data);
	if (*sphere_data && *sphere_data != '\n')
		return (free_obj(sphere), print_err("Sphere has invalid data"));
	return (true);
}

bool	parse_cylinder(char *cylinder_data, t_obj *cylinder)
{
	cylinder->type = CYLINDER;
	cylinder->attrs = malloc(sizeof(float) * 2);
	if (!cylinder->attrs)
		return (free_obj(cylinder), print_err("Can't allocate memory"));	
	skip_info(&cylinder_data);
	if (!get_coordinates(cylinder_data, &cylinder->center))
		return (free_obj(cylinder), print_err("Cylinder has invalid coordinates"));
	skip_info(&cylinder_data);
	if (!get_orientation(cylinder_data, &cylinder->norm_vector))
		return (free_obj(cylinder), print_err("Cylinder has invalid orientation"));
	skip_info(&cylinder_data);
	if (!get_attribute(cylinder_data, &cylinder->attrs[CYLINDER_D_I]))
		return (free_obj(cylinder), print_err("Cylinder has invalid diameter"));
	skip_info(&cylinder_data);
	if (!get_attribute(cylinder_data, &cylinder->attrs[CYLINDER_H_I]))
		return (free_obj(cylinder), print_err("Cylinder has invalid height"));
	skip_info(&cylinder_data);
	if (!get_color(cylinder_data, &cylinder->color))
		return (free_obj(cylinder), print_err("Cylinder has invalid color"));
	skip_info(&cylinder_data);
	if (*cylinder_data && *cylinder_data != '\n')
		return (free_obj(cylinder), print_err("Cylinder has invalid data"));
	return (true);
}

/*
 * co     0,0,0   0,1,0   30         5      255,0,0
 *        center  norm    half_angle height color
 */
bool	parse_cone(char *cone_data, t_obj *cone)
{
	cone->type = CONE;
	cone->attrs = malloc(sizeof(float) * 2);
	if (!cone->attrs)
		return (free_obj(cone), print_err("Can't allocate memory"));	
	skip_info(&cone_data);
	if (!get_coordinates(cone_data, &cone->center))
		return (free_obj(cone), print_err("Cone has invalid coordinates"));
	skip_info(&cone_data);
	if (!get_orientation(cone_data, &cone->norm_vector))
		return (free_obj(cone), print_err("Cone has invalid orientation"));
	skip_info(&cone_data);
	if (!get_attribute(cone_data, &cone->attrs[CONE_A_I]))
		return (free_obj(cone), print_err("Cone has invalid angle"));
	skip_info(&cone_data);
	if (!get_attribute(cone_data, &cone->attrs[CONE_H_I]))
		return (free_obj(cone), print_err("Cone has invalid height"));
	skip_info(&cone_data);
	if (!get_color(cone_data, &cone->color))
		return (free_obj(cone), print_err("Cone has invalid color"));
	skip_info(&cone_data);
	if (*cone_data && *cone_data != '\n')
		return (free_obj(cone), print_err("Cone has invalid data"));
	return (true);
}

/*
 * Here you can just do 'return (parse_plane(...))'
 */
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
	else if (ft_strncmp(obj_data, "cy ", 3) == 0)
	{
		if (!parse_cylinder(obj_data, obj))
			return (false);
	}
	else if (ft_strncmp(obj_data, "co ", 3) == 0)
	{
		if (!parse_cone(obj_data, obj))
			return (false);
	}
	else
		return (true); // TODO: unkown type
	return (append_obj(scene, obj));
}
