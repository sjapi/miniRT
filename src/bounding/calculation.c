/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:11:02 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/23 20:13:13 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "defines.h"
#include "minirt.h"
#include "utils.h"
#include "bounding.h"

static void	calc_sphere_aabb(t_obj *sphere)
{
	float	radius;
	t_vec3	r_vec;

	radius = sphere->attrs[SPHERE_D_I] * 0.5f;
	v_set(&r_vec, radius);
	sphere->aabb_min = v_sub(sphere->center, r_vec);
	sphere->aabb_max = v_add(sphere->center, r_vec);
}

static void	calc_cylinder_aabb(t_obj *cylinder)
{
	float	radius;
	float	h;
	t_vec3	bottom;
	t_vec3	top;
	t_vec3	r_vec;

	radius = cylinder->attrs[CYLINDER_D_I] * 0.5f;
	h = cylinder->attrs[CYLINDER_H_I];
	bottom = cylinder->center;
	top = v_add(cylinder->center, v_scale(cylinder->norm_vector, h));
	cylinder->aabb_min = v_min(&top, &bottom);
	cylinder->aabb_max = v_max(&top, &bottom);
	v_set(&r_vec, radius);
	cylinder->aabb_min = v_sub(cylinder->aabb_min, r_vec);
	cylinder->aabb_max = v_add(cylinder->aabb_max, r_vec);
}

static void	calc_cone_aabb(t_obj *cone)
{
	float	h;
	float	r;
	t_vec3	top;
	t_vec3	bottom;
	t_vec3	r_vec;

	h = cone->attrs[CONE_H_I];
	r = tanf(cone->attrs[CONE_A_I] * (M_PI / 180.0f)) * h;
	top = v_add(cone->center, v_scale(cone->norm_vector, h));
	bottom = cone->center;
	cone->aabb_min = v_min(&top, &bottom);
	cone->aabb_max = v_max(&top, &bottom);
	v_set(&r_vec, r);
	cone->aabb_min = v_sub(cone->aabb_min, r_vec);
	cone->aabb_max = v_add(cone->aabb_max, r_vec);
}

static void	calc_model_aabb(t_obj *model)
{
	t_vec3	min;
	t_vec3	max;
	t_tri	*tri;
	int		i;

	if (!model || !model->mesh || model->mesh->size <= 0)
		return ;
	v_set(&min, INFINITY);
	v_set(&max, -INFINITY);
	i = -1;
	while (++i < model->mesh->size)
	{
		tri = &model->mesh->triangles[i];
		min = v_min(&min, tri->v0);
		max = v_max(&max, tri->v0);
		min = v_min(&min, tri->v1);
		max = v_max(&max, tri->v1);
		min = v_min(&min, tri->v2);
		max = v_max(&max, tri->v2);
	}
	min.y -= 0.01;
	if (min.y == max.y)
		min.y -= 0.001f;
	model->aabb_min = min;
	model->aabb_max = max;
}

void	calculate_aabb(t_obj *obj)
{
	if (obj->type == SPHERE)
		calc_sphere_aabb(obj);
	else if (obj->type == CYLINDER)
		calc_cylinder_aabb(obj);
	else if (obj->type == CONE)
		calc_cone_aabb(obj);
	else if (obj->type == MODEL)
		calc_model_aabb(obj);
}
