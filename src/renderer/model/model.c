/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:54:34 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/24 02:06:18 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

float	intersect_triangle(t_ray *ray, t_tri *triangle)
{
	float	vars[5];
	t_vec3	q;
	t_vec3	s;
	t_vec3	h;

	h = v_cross(ray->direction, triangle->edge2);
	vars[0] = v_dot(triangle->edge1, h);
	if (vars[0] > -1e-6 && vars[0] < 1e-6)
		return (-1);
	vars[1] = 1.0f / vars[0];
	s = v_sub(ray->origin, *triangle->v0);
	vars[2] = vars[1] * v_dot(s, h);
	if (vars[2] < 0.0f || vars[2] > 1.0f)
		return (-1);
	q = v_cross(s, triangle->edge1);
	vars[3] = vars[1] * v_dot(ray->direction, q);
	if (vars[3] < 0.0f || vars[2] + vars[3] > 1.0f)
		return (-1);
	vars[4] = vars[1] * v_dot(triangle->edge2, q);
	if (vars[4] > 1e-6)
		return (vars[4]);
	return (-1);
}

t_vec3	get_model_normal(t_obj *obj, t_vec3 ray_dir, int tri_j)
{
	t_vec3	v[3];
	t_vec3	edges[2];
	t_vec3	normal;

	v[0] = *obj->mesh->triangles[tri_j].v0;
	v[1] = *obj->mesh->triangles[tri_j].v1;
	v[2] = *obj->mesh->triangles[tri_j].v2;
	edges[0] = v_sub(v[1], v[0]);
	edges[1] = v_sub(v[2], v[0]);
	normal = v_cross(edges[0], edges[1]);
	normal = v_normalize(normal);
	if (v_dot(normal, ray_dir) > 0)
		normal = v_scale(normal, -1);
	return (normal);
}

static bool	is_hitable(t_ray *ray, t_obj *obj)
{
	t_vec3	oc;
	float	half_b;
	float	c;

	oc = v_sub(ray->origin, obj->center);
	half_b = v_dot(oc, ray->direction);
	c = v_dot(oc, oc) - obj->bounding_r * obj->bounding_r;
	return (half_b * half_b >= c);
}

float	intersect_model(t_ray *ray, t_obj *obj, t_hit *hit, int *ti)
{
	float	t_min;
	float	t;
	int		i;

	i = -1;
	t_min = -1;
	while (++i < obj->mesh->size)
	{
		t = intersect_triangle(ray, &obj->mesh->triangles[i]);
		if (t > 0 && (t_min < 0 || t < t_min))
		{
			t_min = t;
			*ti = i;
		}
	}
	return (t_min);
}
