/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:35:13 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/11 15:12:52 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

/*
 * can calculate axis earlier
 */
t_vec3	get_cylinder_normal(t_obj *obj, t_vec3 hit_point)
{
	t_vec3	v;
	float		h;
	t_vec3	n;

	v = v_sub(hit_point, obj->center);
	h = v_dot(v, obj->norm_vector);
	if (h <= 1e-4f)
		return (v_scale(obj->norm_vector, -1.0));
	if (h >= obj->attrs[CYLINDER_H_I] - 1e-4f)
		return (obj->norm_vector);
	n = v_sub(v, v_scale(obj->norm_vector, h));
	return (v_normalize(n));
}

float	intersect_cylinder(t_ray *ray, t_obj *obj)
{
	float		radius;
	float		height;
	t_vec3	axis;
	t_vec3	oc;
	float		dv, cov;
	t_vec3	d_perp, oc_perp;
	float		a, b, c;
	float		discrim;
	float		t_side, t1, t2;
	float		t_cap, t_bot, t_top;
	float		t;
	t_vec3	p, top_center, v, v_perp;

	// side
	radius = 0.5f * obj->attrs[CYLINDER_D_I];
	height = obj->attrs[CYLINDER_H_I];
	axis   = obj->norm_vector;
	oc     = v_sub(ray->origin, obj->center);
	dv     = v_dot(ray->direction, axis);
	cov    = v_dot(oc, axis);
	d_perp = v_sub(ray->direction, v_scale(axis, dv));
	oc_perp = v_sub(oc, v_scale(axis, cov));
	a = v_dot(d_perp, d_perp);
	b = 2.0f * v_dot(d_perp, oc_perp);
	c = v_dot(oc_perp, oc_perp) - radius * radius;
	discrim = b * b - 4.0f * a * c;
	t_side = -1.0f;
	if (discrim >= 0.0f && fabsf(a) > 1e-6f)
	{
		float	sq = sqrtf(discrim);

		t1 = (-b - sq) / (2.0f * a);
		t2 = (-b + sq) / (2.0f * a);
		t_side = fminf(t1, t2);
		if (t_side < 1e-6f)
			t_side = fmaxf(t1, t2);
		if (t_side >= 1e-6f)
		{
			p = v_add(ray->origin, v_scale(ray->direction, t_side));
			if (v_dot(v_sub(p, obj->center), axis) < 0.0f ||
				v_dot(v_sub(p, obj->center), axis) > height)
				t_side = -1.0f;
		}
	}

	// top & bottom
	t_cap = -1.0f;
	if (fabsf(dv) > 1e-6f)
	{
		/* bottom */
		t_bot = (-cov) / dv;
		if (t_bot >= 1e-6f)
		{
			p = v_add(ray->origin, v_scale(ray->direction, t_bot));
			v = v_sub(p, obj->center);
			v_perp = v_sub(v, v_scale(axis, v_dot(v, axis)));
			if (v_dot(v_perp, v_perp) <= radius * radius)
				t_cap = t_bot;
		}
		// top
		top_center = v_add(obj->center, v_scale(axis, height));
		t_top = (height - cov) / dv;
		if (t_top >= 1e-6f)
		{
			p = v_add(ray->origin, v_scale(ray->direction, t_top));
			v = v_sub(p, top_center);
			v_perp = v_sub(v, v_scale(axis, v_dot(v, axis)));
			if (v_dot(v_perp, v_perp) <= radius * radius &&
				(t_cap < 0.0f || t_top < t_cap))
				t_cap = t_top;
		}
	}
	// nearest intersection
	if (t_side >= 0.0f && t_cap >= 0.0f)
		t = fminf(t_side, t_cap);
	else if (t_side >= 0.0f)
		t = t_side;
	else
		t = t_cap;
	return (t >= 0.0f ? t : -1.0f);
}
