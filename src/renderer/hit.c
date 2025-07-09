/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:17 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/09 20:38:58 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"

bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit)
{
	int		i;
	float	t;
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
		if (t > 0)
		{
			find = true;
			hit->t = t;
			hit->hit_point = v_add(ray->origin, v_scale(ray->direction, t));
			if (obj->type == PLANE)
				hit->normal = obj->norm_vector;
			else if (obj->type == SPHERE)
				hit->normal = v_normalize(v_sub(hit->hit_point, obj->center));
			hit->obj = obj;
		}
	}
	return (find);
}

int	get_hit_color(t_hit *hit, t_scene *scene)
{
    int obj = hit->obj->color;
    int amb = scene->amb->color;
    float ratio = scene->amb->ratio;

    int r = ((obj >> 16) & 0xFF) * ((amb >> 16) & 0xFF) / 255 * ratio;
    int g = ((obj >> 8) & 0xFF) * ((amb >> 8) & 0xFF) / 255 * ratio;
    int b = (obj & 0xFF) * (amb & 0xFF) / 255 * ratio;

    return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}
