/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:17 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 02:17:18 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include "bounding.h"

static bool	traverse_bvh(t_bvh_node *node, t_ray *ray, t_hit *hit)
{
	t_obj	*obj;
	bool	left_node;
	bool	right_node;

	obj = NULL;
	if (!node || !is_hittable_aabb(ray, &node->aabb_min, &node->aabb_max))
		return (false);
	if (node->object)
	{
		obj = node->object;
		return (is_hittable_object(ray, hit, obj));
	}
	left_node = traverse_bvh(node->left, ray, hit);
	right_node = traverse_bvh(node->right, ray, hit);
	return (left_node || right_node);
}

static void	hit_planes(t_ray *ray, t_scene *scene, t_hit *closest_hit)
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
		hit->normal = get_cylinder_normal(obj, hit->hit_point,
				ray->direction, hit->side);
	else if (obj->type == CONE)
		hit->normal = get_cone_normal(obj, hit->hit_point,
				ray->direction, hit->side);
	else if (obj->type == MODEL)
		hit->normal = get_model_normal(obj, ray->direction, hit->tri_i);
	return (true);
}

bool	is_in_shadow(t_ray *ray, t_scene *scene, float d)
{
	t_hit	hit;

	hit.t = INFINITY;
	hit_planes(ray, scene, &hit);
	if (hit.t > 1e-6 && hit.t < d)
		return (true);
	if (traverse_bvh(scene->bvh, ray, &hit) && hit.t < d)
		return (true);
	return (false);
}
