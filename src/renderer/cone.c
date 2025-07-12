/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:13:12 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/12 11:56:35 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

/*
 * about optimization
 * can calculate data for cone such as angle in rads and apex earlier
 */

/*
 * equation:
 * cone_normal = vec_x - [1 + tan2(angle_rads)] * m * cone_normal_axis_vec
 * vec_x = hit_point - apex_point 
 * m = cone_normal_axis_vec * vec_x
 */
t_vec3 get_cone_normal(t_obj *obj, t_vec3 hit_point)
{
	t_vec3	apex;
	t_vec3	x;
	float		m;
	float		angle_rad;
	t_vec3	result;

	apex = (t_vec3){
		obj->attrs[CONE_AP_X_I],
		obj->attrs[CONE_AP_Y_I],
		obj->attrs[CONE_AP_Z_I]
	};
	x = v_sub(hit_point, apex);
	m = v_dot(obj->norm_vector ,x);
	angle_rad = obj->attrs[CONE_AR_I];
	result = v_sub(x, v_scale(obj->norm_vector, (1 + obj->attrs[CONE_TAN2]) * m));
	return v_normalize(result);
}

float intersect_cone(t_ray *ray, t_obj *obj)
{
	float		dv;
	float		cov;
	float		angle_rad;
	float		cos2a;
	t_vec3	apex;
	t_vec3	co;
	float		a, b, c;
	float		discrim;
	float		t, t1, t2;
	t_vec3	p;
	float		h;

	apex = (t_vec3){
		obj->attrs[CONE_AP_X_I],
		obj->attrs[CONE_AP_Y_I],
		obj->attrs[CONE_AP_Z_I]
	};
	co = v_sub(ray->origin, apex);
	dv = v_dot(ray->direction, obj->norm_vector);
	cov = v_dot(co, obj->norm_vector);
	angle_rad = obj->attrs[CONE_AR_I];
	cos2a = obj->attrs[CONE_COS2];
	a = v_dot(ray->direction, ray->direction) - (dv * dv) / cos2a;
	b = 2 * (v_dot(ray->direction, co) - (dv * cov) / cos2a);
	c = v_dot(co, co) - (cov * cov) / cos2a;
	discrim = b * b - 4.0 * a * c;
	if (discrim < 0)
		return (-1);
	t1 = (-b - sqrtf(discrim)) / (2.0 * a);
	t2 = (-b + sqrtf(discrim)) / (2.0 * a);
	t = fminf(t1, t2);
	if (t < 0.0)
		t = fmaxf(t1, t2);
	if (t < 0.0)
		return (-1);
	p = v_add(ray->origin, v_scale(ray->direction, t));
	h = v_dot(v_sub(p, obj->center), obj->norm_vector);
	if (h < 0.0f || h > obj->attrs[CONE_H_I])
		return (-1);
	return (t);
}
