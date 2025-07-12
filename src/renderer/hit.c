/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:17 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/12 14:18:44 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"



/*
 * need to add normal for cylinder - done
 * need to add optimization for cylinder & cone
 */
bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit, bool shadow)
{
	int		i;
	float	t;
	float	closest = 270000;
	t_obj	*obj;
	bool	find = false;
	char	side;

	i = -1;
	while (++i < info->scene->objs_count)
	{
		t = -1;
		obj = &info->scene->objs[i];
		//if (!is_hitable(ray, obj))
		//	continue ;
		if (obj->type == PLANE)
			t = intersect_plane(ray, obj);
		else if (obj->type == SPHERE)
			t = intersect_sphere(ray, obj);
		else if (obj->type == CYLINDER)
			t = intersect_cylinder(ray, obj, &side);
		else if (obj->type == CONE)
			t = intersect_cone(ray, obj, &side);
		if (t > 0 && t < closest)
		{
			find = true;
			hit->t = t;
			closest = t;
			hit->hit_point = v_add(ray->origin, v_scale(ray->direction, t));
			if (shadow)
				continue ;
			if (obj->type == PLANE)
				hit->normal = obj->norm_vector;
			else if (obj->type == SPHERE)
				hit->normal = v_normalize(v_sub(hit->hit_point, obj->center));
			else if (obj->type == CYLINDER)
				hit->normal = get_cylinder_normal(obj, hit->hit_point, ray->direction, side);
			else if (obj->type == CONE)
				hit->normal = get_cone_normal(obj, hit->hit_point, ray->direction, side);
			hit->obj = obj;
		}
	}
	return (find);
}
