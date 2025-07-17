/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:17 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/17 15:28:37 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"

bool is_hitable(t_ray *ray, t_obj *obj)
{
    t_vec3 oc = v_sub(ray->origin, obj->center);
    float half_b = v_dot(oc, ray->direction);
    float c = v_dot(oc, oc) - obj->bounding_r * obj->bounding_r;
    return (half_b * half_b >= c);
}

/*
 * need to add normal for cylinder - done
 * need to add optimization for cylinder & cone
 */
bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit, bool shadow_hit)
{
    int i;
	int	tri_i = -1;
    float t;
    float closest = 270000.0f;
    t_obj *obj;
    bool find = false;
	bool	tmp_reverse = false;

    i = -1;
    while (++i < info->scene->objs_count)
    {
        t = -1.0f;
        obj = &info->scene->objs[i];
		if (obj->type == CYLINDER && !is_hitable(ray, obj))
			continue ;
        if (obj->type == PLANE)
            t = intersect_plane(ray, obj, &tmp_reverse);
        else if (obj->type == SPHERE)
            t = intersect_sphere(ray, obj, &tmp_reverse);
        else if (obj->type == CYLINDER)
            t = intersect_cylinder(ray, obj, &hit->side, &tmp_reverse);
        else if (obj->type == CONE)
            t = intersect_cone(ray, obj, &hit->side, &tmp_reverse);
        else if (obj->type == MODEL)
        	t = intersect_model(ray, obj, hit, &tri_i, &tmp_reverse);

        if (t > 0 && t < closest)
        {
            find = true;
            closest = t;
            hit->t = t;
			hit->reverse = tmp_reverse;
            hit->hit_point = v_add(ray->origin, v_scale(ray->direction, t));

            if (shadow_hit)
                continue;

            if (obj->type == PLANE)
                hit->normal = obj->norm_vector;
            else if (obj->type == SPHERE)
                hit->normal = v_normalize(v_sub(hit->hit_point, obj->center));
            else if (obj->type == CYLINDER)
                hit->normal = get_cylinder_normal(obj, hit->hit_point, ray->direction, hit->side);
            else if (obj->type == CONE)
                hit->normal = get_cone_normal(obj, hit->hit_point, ray->direction, hit->side);
			else if (obj->type == MODEL)
				hit->normal = get_model_normal(obj, hit->hit_point, ray->direction, tri_i);
            hit->obj = obj;
			
        }
    }
	if (0 && find && !shadow_hit)
	{
			hit->t = closest;
			hit->reverse = tmp_reverse;
            hit->hit_point = v_add(ray->origin, v_scale(ray->direction, closest));

			obj = hit->obj;


            if (obj->type == PLANE)
                hit->normal = obj->norm_vector;
            else if (obj->type == SPHERE)
                hit->normal = v_normalize(v_sub(hit->hit_point, obj->center));
            else if (obj->type == CYLINDER)
                hit->normal = get_cylinder_normal(obj, hit->hit_point, ray->direction, hit->side);
            else if (obj->type == CONE)
                hit->normal = get_cone_normal(obj, hit->hit_point, ray->direction, hit->side);
			else if (obj->type == MODEL)
				hit->normal = get_model_normal(obj, hit->hit_point, ray->direction, tri_i);
	}
    return find;
}

