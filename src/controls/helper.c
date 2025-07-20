/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:56:28 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/20 19:09:58 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "minirt.h"
#include "utils.h"
#include <stdbool.h>

static void	adjust_attr(float *attr, float delta)
{
	*attr = clampf(*attr + delta, 0.1f, 1e9f);
}

bool	adjust_sphere(t_obj *obj, int key)
{
	if (key == KEY_RIGHT)
		adjust_attr(&obj->attrs[SPHERE_D_I], +0.1f);
	else if (key == KEY_LEFT)
		adjust_attr(&obj->attrs[SPHERE_D_I], -0.1f);
	else
		return (false);
	return (true);
}

bool	adjust_cylinder(t_obj *obj, int key)
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
	return (true);
}

bool	adjust_cone(t_obj *obj, int key)
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
	return (true);
}
