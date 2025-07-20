/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:57:47 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/20 16:58:04 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"
#include "utils.h"
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
	t_vec3	v;
	t_vec3	res;
	float	angle;

	v = obj->norm_vector;
	angle = 0.1f;
	if (key == KEY_X)
	{
		res.x = v.x;
		res.y = v.y * cosf(angle) - v.z * sinf(angle);
		res.z = v.y * sinf(angle) + v.z * cosf(angle);
	}
	else if (key == KEY_Y)
	{
		res.x = v.x * cosf(angle) + v.z * sinf(angle);
		res.y = v.y;
		res.z = -v.x * sinf(angle) + v.z * cosf(angle);
	}
	else if (key == KEY_Z)
	{
		res.x = v.x * cosf(angle) - v.y * sinf(angle);
		res.y = v.x * sinf(angle) + v.y * cosf(angle);
		res.z = v.z;
	}
	else
		return (false);
	obj->norm_vector = res;
	return (true);
}

static void adjust_attr(float *attr, float delta)
{
	*attr = clampf(*attr + delta, 0.1f, 1e9f);
}

bool resize_obj(t_obj *obj, int key)
{
	if (obj->type == SPHERE)
	{
		if (key == KEY_RIGHT)
			adjust_attr(&obj->attrs[SPHERE_D_I], +0.1f);
		else if (key == KEY_LEFT)
			adjust_attr(&obj->attrs[SPHERE_D_I], -0.1f);
		else
			return (false);
	}
	else if (obj->type == CYLINDER)
	{
		if (key == KEY_RIGHT)
			adjust_attr(&obj->attrs[CYLINDER_D_I], +0.1f);
		else if (key == KEY_LEFT)
			adjust_attr(&obj->attrs[CYLINDER_D_I], -0.1f);
		else if (key == KEY_TOP)
			adjust_attr(&obj->attrs[CYLINDER_H_I], +0.1f);
		else if (key == KEY_BOTTOM)
			adjust_attr(&obj->attrs[CYLINDER_H_I], -0.1f);
		else
			return (false);
	}
	else if (obj->type == CONE)
	{
		if (key == KEY_RIGHT)
			obj->attrs[CONE_A_I] = clampf(obj->attrs[CONE_A_I] - 1, 1, 179);
		else if (key == KEY_LEFT)
			obj->attrs[CONE_A_I] = clampf(obj->attrs[CONE_A_I] + 1, 1, 179);
		else if (key == KEY_TOP)
			adjust_attr(&obj->attrs[CONE_H_I], +0.1f);
		else if (key == KEY_BOTTOM)
			adjust_attr(&obj->attrs[CONE_H_I], -0.1f);
		else
			return (false);
	}
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
