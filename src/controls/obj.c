/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:57:47 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/20 20:01:49 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"
#include "utils.h"
#include "controls.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

bool	translate_obj(t_obj *obj, int key)
{
	float	val;

	val = 0.2;
	if (key == KEY_Q)
		obj->center.x += val;
	else if (key == KEY_A)
		obj->center.x -= val;
	else if (key == KEY_W)
		obj->center.y += val;
	else if (key == KEY_S)
		obj->center.y -= val;
	else if (key == KEY_E)
		obj->center.z += val;
	else if (key == KEY_D)
		obj->center.z -= val;
	else
		return (false);
	return (true);
}

bool	rotate_obj(t_obj *obj, int key)
{
	t_vec3	res;
	float	angle;

	res = obj->norm_vector;
	angle = 0.1f;
	if (key == KEY_X)
	{
		res.y = res.y * cosf(angle) - res.z * sinf(angle);
		res.z = res.y * sinf(angle) + res.z * cosf(angle);
	}
	else if (key == KEY_Y)
	{
		res.x = res.x * cosf(angle) + res.z * sinf(angle);
		res.z = -res.x * sinf(angle) + res.z * cosf(angle);
	}
	else if (key == KEY_Z)
	{
		res.x = res.x * cosf(angle) - res.y * sinf(angle);
		res.y = res.x * sinf(angle) + res.y * cosf(angle);
	}
	else
		return (false);
	obj->norm_vector = res;
	return (true);
}

bool	resize_obj(t_obj *obj, int key)
{
	if (obj->type == SPHERE && !adjust_sphere(obj, key))
		return (false);
	else if (obj->type == CYLINDER && !adjust_cylinder(obj, key))
		return (false);
	else if (obj->type == CONE && !adjust_cone(obj, key))
		return (false);
	else
		return (false);
	calculate_bounding(obj);
	return (true);
}

void	select_obj(t_obj *obj, t_scene *scene)
{
	obj->selected = true;
	scene->selected = obj;
}

void	deselect_obj(t_scene *scene)
{
	scene->selected->selected = false;
	scene->selected = NULL;
}
