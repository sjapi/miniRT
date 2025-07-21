/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:11:02 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/21 17:38:38 by haaghaja         ###   ########.fr       */
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
	r_vec.x = radius;
	r_vec.y = radius;
	r_vec.z = radius;
	sphere->aabb_min = v_sub(sphere->center, r_vec);
	sphere->aabb_max = v_add(sphere->center, r_vec);
}

static void calc_cylinder_aabb(t_obj *cylinder)
{
	float	height;
	float	radius;
	t_vec3	half_height;
	t_vec3	top;
	t_vec3	bottom;
	t_vec3	r_vec;

	height = cylinder->attrs[CYLINDER_H_I];
	radius = cylinder->attrs[CYLINDER_D_I];
	half_height = v_scale(cylinder->norm_vector, height * 0.5f);
	top = v_add(cylinder->center, half_height);
	bottom = v_sub(cylinder->center, half_height);
	cylinder->aabb_min = v_min(&top, &bottom);
	cylinder->aabb_max = v_max(&top, &bottom);
	r_vec.x = radius;
	r_vec.y = radius;
	r_vec.z = radius;
	cylinder->aabb_min = v_sub(cylinder->aabb_min, r_vec);
	cylinder->aabb_max = v_add(cylinder->aabb_max, r_vec);
}

static void calc_cone_aabb(t_obj *cone)
{
	float   h;
	float   r;
	t_vec3  half_height;
	t_vec3  top;
	t_vec3  bottom;
	t_vec3  r_vec;

	h = cone->attrs[CONE_H_I];
	r = tan(cone->attrs[CONE_A_I] * (M_PI / 180.0f)) * (h);
	half_height = v_scale(cone->norm_vector, h * 0.5f);
	top = v_add(cone->center, half_height);
	bottom = v_sub(cone->center, half_height);
	cone->aabb_min = v_min(&top, &bottom);
	cone->aabb_max = v_max(&top, &bottom);
	r_vec.x = r;
	r_vec.y = r;
	r_vec.z = r;
	cone->aabb_min = v_sub(cone->aabb_min, r_vec);
	cone->aabb_max = v_add(cone->aabb_max, r_vec);
}


static void	calc_model_aabb(t_obj *model)
{
	if (!model || !model->mesh || model->mesh->size <= 0)
		return;
	t_vec3	min = { INFINITY, INFINITY, INFINITY };
	t_vec3	max = { -INFINITY, -INFINITY, -INFINITY };
	for (int i = 0; i < model->mesh->size; ++i)
	{
		t_tri	*tri = &model->mesh->triangles[i];
		t_vec3 points[3] = { *tri->v0, *tri->v1, *tri->v2 };
		for (int j = 0; j < 3; ++j)
		{
			t_vec3	p = points[j];
			min = v_min(&min, &p);
			max = v_max(&max, &p);
		}
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
