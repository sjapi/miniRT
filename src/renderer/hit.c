/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:17 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/11 00:22:16 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"

/*
 * need to add normal for cylinder
 */
bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit)
{
	int		i;
	float	t;
	float	closest = 270000;
	t_obj	*obj;
	bool	find = false;

	i = -1;
	while (++i < info->scene->objs_count)
	{
		t = -1;
		obj = &info->scene->objs[i];
		if (obj->type == PLANE)
			t = intersect_plane(ray, obj);
		else if (obj->type == SPHERE)
			t = intersect_sphere(ray, obj);
		else if (obj->type == CYLINDER)
			t = intersect_cylinder(ray, obj);
		else if (obj->type == CONE)
			t = intersect_cone(ray, obj);
		if (t > 0 && t < closest)
		{
			closest = t;
			find = true;
			hit->t = t;
			hit->hit_point = v_add(ray->origin, v_scale(ray->direction, t));
			if (obj->type == PLANE)
				hit->normal = obj->norm_vector;
			else if (obj->type == SPHERE)
				hit->normal = v_normalize(v_sub(hit->hit_point, obj->center));
			else if (obj->type == CONE)
			{
				hit->normal = get_cone_normal(obj, hit->hit_point);
			}
			hit->obj = obj;
		}
	}
	return (find);
}
