/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:34:24 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/23 17:55:02 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

t_vec3	get_cone_normal(t_obj *obj, t_vec3 hit_point, t_vec3 ray_dir, char side)
{
	t_vec3	apex;
	t_vec3	normal;
	t_vec3	x;
	t_vec3	n;
	float	m;

	apex = (t_vec3){obj->attrs[CONE_AP_X_I], obj->attrs[CONE_AP_Y_I],
		obj->attrs[CONE_AP_Z_I]};
	if (side == HIT_BOTTOM)
		normal = obj->norm_vector;
	else
	{
		x = v_sub(hit_point, apex);
		m = v_dot(obj->norm_vector, x);
		n = v_sub(x, v_scale(obj->norm_vector,
					(1 + obj->attrs[CONE_TAN2]) * m));
		normal = v_normalize(n);
	}
	if (v_dot(normal, ray_dir) > 0)
		normal = v_scale(normal, -1);
	return (normal);
}

/* 0 denom, 1 t, 2 radius, 3 dist_sq */
static float	intersect_cone_base(t_ray *ray, t_obj *obj)
{
	t_vec3	apex;
	t_vec3	base_center;
	float	norm[4];
	t_vec3	p;
	t_vec3	diff;

	apex = (t_vec3){obj->attrs[CONE_AP_X_I],
		obj->attrs[CONE_AP_Y_I], obj->attrs[CONE_AP_Z_I]};
	base_center = v_sub(apex, v_scale(obj->norm_vector, obj->attrs[CONE_H_I]));
	norm[0] = v_dot(ray->direction, obj->norm_vector);
	if (fabsf(norm[0]) < 1e-6)
		return (-1.0);
	norm[1] = v_dot(v_sub(base_center, ray->origin), obj->norm_vector);
	norm[1] /= norm[0];
	if (norm[1] < 0.0f)
		return (-1.0);
	p = v_add(ray->origin, v_scale(ray->direction, norm[1]));
	diff = v_sub(p, base_center);
	norm[2] = obj->attrs[CONE_H_I] * tanf(obj->attrs[CONE_AR_I]);
	norm[3] = v_dot(diff, diff);
	if (norm[3] <= norm[2] * norm[2])
		return (norm[1]);
	return (-1.0);
}

static float	get_t_surface(float norm[6], t_obj *obj,
					t_ray *ray, t_vec3 apex)
{
	float	t[3];
	float	h;

	t[0] = (-norm[3] - sqrtf(norm[5])) / (2.0f * norm[2]);
	t[1] = (-norm[3] + sqrtf(norm[5])) / (2.0f * norm[2]);
	t[2] = -1.0f;
	if (t[0] > 0.0f)
		t[2] = t[0];
	if (t[1] > 0.0f && (t[2] < 0.0f || t[1] < t[2]))
		t[2] = t[1];
	if (t[2] < 0.0f)
		return (-1.0);
	h = v_dot(v_sub(apex, v_add(ray->origin,
					v_scale(ray->direction, t[2]))), obj->norm_vector);
	if (h < 0.0f || h > obj->attrs[CONE_H_I])
		return (-1.0);
	return (t[2]);
}

/* dv 0, cov 1, a 2, b 3, c 4, disc 5, */
static float	intersect_cone_surface(t_ray *ray, t_obj *obj)
{
	t_vec3	apex;
	t_vec3	co;
	float	norm[6];

	apex = (t_vec3){obj->attrs[CONE_AP_X_I], obj->attrs[CONE_AP_Y_I],
		obj->attrs[CONE_AP_Z_I]};
	co = v_sub(ray->origin, apex);
	norm[0] = v_dot(ray->direction, obj->norm_vector);
	norm[1] = v_dot(co, obj->norm_vector);
	norm[2] = v_dot(ray->direction, ray->direction);
	norm[2] -= (norm[0] * norm[0]) / obj->attrs[CONE_COS2];
	norm[3] = 2.0 * (v_dot(ray->direction, co) - (norm[0] * norm[1]) / obj->attrs[CONE_COS2]);
	norm[4] = v_dot(co, co) - (norm[1] * norm[1]) / obj->attrs[CONE_COS2];
	norm[5] = norm[3] * norm[3] - 4.0 * norm[2] * norm[4];
	if (norm[5] < 0.0)
		return (-1.0);
	return (get_t_surface(norm, obj, ray, apex));
}

float	intersect_cone(t_ray *ray, t_obj *obj, char *side, bool *reverse)
{
	float	surface;
	float	base;
	float	t;
	t_vec3	hit_point;

	surface = intersect_cone_surface(ray, obj);
	base = intersect_cone_base(ray, obj);
	t = -1.0;
	if (surface > 0)
	{
		t = surface;
		*side = HIT_SURFACE;
	}
	if (base > 0 && (t < 0 || base < t))
	{
		t = base;
		*side = HIT_BOTTOM;
	}
	if (t > 0 && (obj->checkerboard || obj->selected))
	{
		hit_point = v_add(ray->origin, v_scale(ray->direction, t));
		*reverse = cone_checkerboard(hit_point, obj, *side == HIT_BOTTOM);
	}
	return (t);
}
