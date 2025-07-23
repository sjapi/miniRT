/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:17 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/23 16:16:23 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include "bounding.h"

static bool is_hittable_aabb(t_ray *ray, t_vec3 *box_min, t_vec3 *box_max)
{
    float tmin = -INFINITY;
    float tmax = INFINITY;

    for (int i = 0; i < 3; i++)
    {
        float invD = 1.0f / ((&ray->direction.x)[i]);
        float t0 = (((&box_min->x)[i]) - ((&ray->origin.x)[i])) * invD;
        float t1 = (((&box_max->x)[i]) - ((&ray->origin.x)[i])) * invD;

        if (invD < 0.0f)
        {
            float tmp = t0;
            t0 = t1;
            t1 = tmp;
        }

        if (t0 > tmin)
            tmin = t0;
        if (t1 < tmax)
            tmax = t1;

        if (tmax <= tmin)
            return false;
    }
    return true;
}

static bool traverse_bvh(t_bvh_node *node, t_ray *ray, t_hit *closest_hit)
{
    if (!node || !is_hittable_aabb(ray, &node->aabb_min, &node->aabb_max))
        return false;
    bool hit = false;
    if (node->object) // Leaf node
    {
        t_obj *obj = node->object;
        float t = INFINITY;
        bool tmp_reverse = false;
        if (obj->type == SPHERE)
            t = intersect_sphere(ray, obj, &tmp_reverse);
        else if (obj->type == CYLINDER)
            t = intersect_cylinder(ray, obj, &closest_hit->side, &tmp_reverse);
        else if (obj->type == CONE)
            t = intersect_cone(ray, obj, &closest_hit->side, &tmp_reverse);
        else if (obj->type == MODEL)
            t = intersect_model(ray, obj, closest_hit, &closest_hit->tri_i, &tmp_reverse);
        if (t > 0.001f && t < closest_hit->t)
        {
            closest_hit->t = t;
            closest_hit->obj = obj;
			closest_hit->reverse = tmp_reverse;
            hit = true;
        }
        return hit;
    }
    bool hit_left = traverse_bvh(node->left, ray, closest_hit);
    bool hit_right = traverse_bvh(node->right, ray, closest_hit);
    return hit_left || hit_right;
}

static void hit_planes(t_ray *ray, t_scene *scene, t_hit *closest_hit)
{
    float	t;
    bool	tmp_reverse;
	int		i;
	t_obj	*obj;

	i = -1;
	while (++i < scene->objs_count)
    {
        obj = &scene->objs[i];
        if (obj->type != PLANE)
            continue ;
        t = intersect_plane(ray, obj, &tmp_reverse);
        if (t > 0.001f && t < closest_hit->t)
        {
            closest_hit->t = t;
            closest_hit->obj = obj;
			closest_hit->reverse = tmp_reverse;
        }
    }
}

/*
 * need to add normal for cylinder - done
 * need to add optimization for cylinder & cone
 */
bool	find_hit(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_obj	*obj;

   	hit->t = INFINITY;
	hit_planes(ray, scene, hit);
	if (!traverse_bvh(scene->bvh, ray, hit) && hit->t == INFINITY)
		return (false);
	obj = hit->obj;
	hit->hit_point = v_add(ray->origin, v_scale(ray->direction, hit->t));
	if (hit->t > 100)
		return (false);
	if (obj->type == PLANE)
		hit->normal = obj->norm_vector;
	else if (obj->type == SPHERE)
		hit->normal = v_normalize(v_sub(hit->hit_point, obj->center));
	else if (obj->type == CYLINDER)
		hit->normal = get_cylinder_normal(obj, hit->hit_point, ray->direction, hit->side);
	else if (obj->type == CONE)
		hit->normal = get_cone_normal(obj, hit->hit_point, ray->direction, hit->side);
	else if (obj->type == MODEL)
		hit->normal = get_model_normal(obj, hit->hit_point, ray->direction, hit->tri_i);
	return (true);
}

bool is_in_shadow(t_ray *ray, t_scene *scene, float d)
{
	t_hit	hit;

   	hit.t = INFINITY;
	hit_planes(ray, scene, &hit);
	if (hit.t > 1e-6 && hit.t < d)
		return (true);
	if (traverse_bvh(scene->bvh, ray, &hit) && hit.t > 1e-6 && hit.t < d)
		return (true);
	return (false);
}

