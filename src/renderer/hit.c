/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:17 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/14 16:17:11 by azolotar         ###   ########.fr       */
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
bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit, bool shadow_hit)
{
    int i;
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
        if (obj->type == PLANE)
            t = intersect_plane(ray, obj, &tmp_reverse);
        else if (obj->type == SPHERE)
            t = intersect_sphere(ray, obj, &tmp_reverse);
        else if (obj->type == CYLINDER)
            t = intersect_cylinder(ray, obj, &hit->side, &tmp_reverse);
        else if (obj->type == CONE)
            t = intersect_cone(ray, obj, &hit->side, &tmp_reverse);

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

            hit->obj = obj;
        }
    }
    return find;
}

