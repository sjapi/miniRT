/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:56:16 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 15:46:31 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

t_vec3	get_cylinder_normal(t_obj *obj, t_vec3 hit_point,
			t_vec3 ray_dir, char side)
{
	t_vec3	v;
	float	h;
	t_vec3	n;
	t_vec3	normal;

	if (side == HIT_TOP)
		normal = obj->norm_vector;
	else if (side == HIT_BOTTOM)
		normal = v_scale(obj->norm_vector, -1);
	else
	{
		v = v_sub(hit_point, obj->center);
		h = v_dot(v, obj->norm_vector);
		n = v_sub(v, v_scale(obj->norm_vector, h));
		normal = v_normalize(n);
	}
	if (v_dot(normal, ray_dir) > 0)
		normal = v_scale(normal, -1);
	return (normal);
}

static float	get_t_surface(t_obj *cyl, float norm[6])
{
	float	t[3];
	float	m;

	t[0] = (-norm[3] - sqrtf(norm[5])) / (2 * norm[2]);
	t[1] = (-norm[3] + sqrtf(norm[5])) / (2 * norm[2]);
	t[2] = -1.0;
	if (t[0] > 0)
	{
		m = norm[1] + t[0] * norm[0];
		if (m >= 0 && m <= cyl->attrs[CYLINDER_H_I])
			t[2] = t[0];
	}
	if (t[1] > 0)
	{
		m = norm[1] + t[1] * norm[0];
		if (m >= 0 && m <= cyl->attrs[CYLINDER_H_I])
		{
			if (t[2] < 0 || t[1] < t[2])
				t[2] = t[1];
		}
	}
	return (t[2]);
}

/* 0 - dv, 1 - ocv, 2 - a, 3 - b, 4 - c, 5 - disc, 6 - cyl radius*/
float	intersect_surface(t_ray *ray, t_obj *cyl)
{
	t_vec3	oc;
	float	norm[7];

	oc = v_sub(ray->origin, cyl->center);
	norm[0] = v_dot(ray->direction, cyl->norm_vector);
	norm[1] = v_dot(oc, cyl->norm_vector);
	norm[2] = v_dot(ray->direction, ray->direction) - norm[0] * norm[0];
	norm[3] = 2.0 * (v_dot(ray->direction, oc) - norm[0] * norm[1]);
	norm[6] = cyl->attrs[CYLINDER_D_I] * 0.5;
	norm[4] = v_dot(oc, oc) - norm[1] * norm[1];
	norm[4] -= norm[6] * norm[6];
	norm[5] = norm[3] * norm[3] - 4 * norm[2] * norm[4];
	if (norm[5] < 0)
		return (-1.0);
	return (get_t_surface(cyl, norm));
}

float	intersect_base(t_ray *ray, t_obj *cyl, int base)
{
	t_vec3	base_center;
	float	denom;
	float	t;
	t_vec3	diff;
	float	dist_sq;

	base_center = cyl->center;
	if (base == HIT_TOP)
		base_center = v_add(cyl->center,
				v_scale(cyl->norm_vector, cyl->attrs[CYLINDER_H_I]));
	denom = v_dot(ray->direction, cyl->norm_vector);
	if (fabsf(denom) < 1e-6)
		return (-1.0);
	t = v_dot(v_sub(base_center, ray->origin), cyl->norm_vector) / denom;
	if (t < 0)
		return (-1.0);
	diff = v_sub(v_add(ray->origin, v_scale(ray->direction, t)), base_center);
	dist_sq = v_dot(diff, diff);
	if (dist_sq <= (cyl->attrs[CYLINDER_D_I] / 2.0)
		* (cyl->attrs[CYLINDER_D_I] / 2.0))
		return (t);
	return (-1.0);
}

void	intersect_cyl_all(float *t, t_obj *obj, t_ray *ray, char *side);

float	intersect_cylinder(t_ray *ray, t_obj *obj, char *side, bool *reverse)
{
	float	t;
	t_vec3	hit_point;

	intersect_cyl_all(&t, obj, ray, side);
	*reverse = false;
	if (t > 0 && (obj->checkerboard || obj->selected))
	{
		hit_point = v_add(ray->origin, v_scale(ray->direction, t));
		if (*side == HIT_SURFACE)
			*reverse = cylinder_checkerboard(hit_point, obj, false);
		else if (*side == HIT_TOP || *side == HIT_BOTTOM)
			*reverse = cylinder_checkerboard(hit_point, obj, true);
	}
	return (t);
}
