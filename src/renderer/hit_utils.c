/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:00:20 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/23 22:04:00 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <math.h>
#include "renderer.h"
#include "minirt.h"
#include "defines.h"

static bool	axis_min_max(float t0, float t1, float *tmm, float invd)
{
	float	tmp;

	if (invd < 0.0f)
	{
		tmp = t0;
		t0 = t1;
		t1 = tmp;
	}
	if (t0 > tmm[0])
		tmm[0] = t0;
	if (t1 < tmm[1])
		tmm[1] = t1;
	return (tmm[1] > tmm[0]);
}

bool	is_hittable_aabb(t_ray *ray, t_vec3 *box_min, t_vec3 *box_max)
{
	float	tmm[2];
	float	invd;

	tmm[0] = -INFINITY;
	tmm[1] = INFINITY;
	invd = 1 / ray->direction.x;
	if (!axis_min_max((box_min->x - ray->origin.x) * invd,
			(box_max->x - ray->origin.x) * invd, tmm, invd))
		return (false);
	invd = 1 / ray->direction.y;
	if (!axis_min_max((box_min->y - ray->origin.y) * invd,
			(box_max->y - ray->origin.y) * invd, tmm, invd))
		return (false);
	invd = 1 / ray->direction.z;
	if (!axis_min_max((box_min->z - ray->origin.z) * invd,
			(box_max->z - ray->origin.z) * invd, tmm, invd))
		return (false);
	return (true);
}

bool	is_hittable_object(t_ray *ray, t_hit *hit, t_obj *obj)
{
	float	t;
	bool	tr;

	if (obj->type == SPHERE)
		t = intersect_sphere(ray, obj, &tr);
	else if (obj->type == CYLINDER)
		t = intersect_cylinder(ray, obj, &hit->side, &tr);
	else if (obj->type == CONE)
		t = intersect_cone(ray, obj, &hit->side, &tr);
	else if (obj->type == MODEL)
		t = intersect_model(ray, obj, hit, &hit->tri_i, &tr);
	if (t > 1e-3 && t < hit->t)
	{
		hit->t = t;
		hit->obj = obj;
		hit->reverse = tr;
		return (true);
	}
	return (false);
}
