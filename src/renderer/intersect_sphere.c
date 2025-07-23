/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:55:36 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/23 15:20:50 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

static float	get_t(t_ray *ray, t_obj *sphere)
{
	float	b;
	float	c;
	t_vec3	oc;
	float	disc;
	float	t[3];

	oc = v_sub(ray->origin, sphere->center);
	b = 2.0f * v_dot(oc, ray->direction);
	c = v_dot(oc, oc) - sphere->attrs[SPHERE_R_I] * sphere->attrs[SPHERE_R_I];
	disc = b * b - 4 * 1.0 * c;
	if (disc < 0)
		return (-1);
	t[0] = (-b - sqrtf(disc)) / (2 * 1.0);
	t[1] = (-b + sqrtf(disc)) / (2 * 1.0);
	if (t[0] > 1e-6)
		t[2] = t[0];
	else if (t[1] > 1e-6)
		t[2] = t[1];
	else
		return (-1);
	return (t[2]);
}

float	intersect_sphere(t_ray *ray, t_obj *sphere, bool *reverse)
{
	float	t;
	t_vec3	hit_point;

	t = get_t(ray, sphere);
	if (t == -1)
		return (-1);
	hit_point = v_add(ray->origin, v_scale(ray->direction, t));
	*reverse = false;
	if (sphere->checkerboard || sphere->selected)
		*reverse = sphere_checkerboard(hit_point, sphere);
	return (t);
}
