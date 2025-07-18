/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:54:34 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/17 15:48:25 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

float intersect_triangle(t_ray *ray, t_tri *triangle, bool *reverse)
{
	(void)reverse; // not used for now

	const float EPSILON = 1e-6;
	t_vec3 v0 = *triangle->v0;
	t_vec3 v1 = *triangle->v1;
	t_vec3 v2 = *triangle->v2;
	t_vec3 edge1 = triangle->edge1;
	t_vec3 edge2 = triangle->edge2;

	t_vec3 h = v_cross(ray->direction, edge2);
	float a = v_dot(edge1, h);

	if (a > -EPSILON && a < EPSILON)
		return -1; // parallel

	float f = 1.0f / a;
	t_vec3 s = v_sub(ray->origin, v0);
	float u = f * v_dot(s, h);
	if (u < 0.0f || u > 1.0f)
		return -1;

	t_vec3 q = v_cross(s, edge1);
	float v = f * v_dot(ray->direction, q);
	if (v < 0.0f || u + v > 1.0f)
		return -1;

	float t = f * v_dot(edge2, q);
	if (t > EPSILON)
		return t;

	return -1;
}

t_vec3	get_model_normal(t_obj *obj, t_vec3 hit_point, t_vec3 ray_dir, int tri_j)
{
	t_vec3 v0 = *obj->mesh->triangles[tri_j].v0;
	t_vec3 v1 = *obj->mesh->triangles[tri_j].v1;
	t_vec3 v2 = *obj->mesh->triangles[tri_j].v2;

	t_vec3 edge1 = v_sub(v1, v0);
	t_vec3 edge2 = v_sub(v2, v0);

	t_vec3 normal = v_cross(edge1, edge2);
	normal = v_normalize(normal);

	if (v_dot(normal, ray_dir) > 0)
		normal = v_scale(normal, -1);

	return normal;
}

static bool is_hitable(t_ray *ray, t_obj *obj)
{
    t_vec3 oc = v_sub(ray->origin, obj->center);
    float half_b = v_dot(oc, ray->direction);
    float c = v_dot(oc, oc) - obj->bounding_r * obj->bounding_r;
    return (half_b * half_b >= c);
}

float intersect_model(t_ray *ray, t_obj *obj, t_hit *hit, int *tj, bool *reverse)
{
	float t_min = -1;
	int j = 0;

	while (j < obj->mesh->size)
	{
		float t = intersect_triangle(ray, &obj->mesh->triangles[j], reverse);
		if (t > 0 && (t_min < 0 || t < t_min))
		{
			t_min = t;
			*tj = j;
		}
		j++;
	}
	return t_min;
}

