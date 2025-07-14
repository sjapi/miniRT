/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:29:34 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/14 16:21:14 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include <math.h>

bool	sphere_checkerboard(t_vec3 hit_point, t_obj *sphere)
{
	t_vec3	normal;
	float	longitude;
	float	latitude;
	int		cell_x_i;
	int		cell_y_i;

	if (!sphere->checkerboard)
		return (false);
	normal = v_normalize(v_sub(hit_point, sphere->center));
	longitude  = 0.5f + (atan2f(normal.z, normal.x) / (2 * M_PI));
	latitude = 0.5f - (asinf(normal.y) / M_PI);
	cell_x_i = (int)(longitude * CHECKER_X_COUNT);
	cell_y_i = (int)(latitude * CHECKER_Y_COUNT);
	return ((cell_x_i + cell_y_i) % 2 == 0);
}

bool cylinder_checkerboard(t_vec3 hit_point, t_obj *cyl, bool base)
{
	t_vec3	axis;
	t_vec3	diff;
	t_vec3	local;
	t_vec3	tangent;
	t_vec3	bitangent;
	t_vec3	base_center;
	float	radius;
	float	norm_rad;
	float	theta;
	float	height;
	float	x_local;
	float	y_local;
	int		cell_x_i;
	int		cell_y_i;


	axis = cyl->norm_vector;
	diff = v_sub(hit_point, cyl->center);

	if (base)
	{
		if (v_dot(diff, axis) > cyl->attrs[CYLINDER_H_I] * 0.5f)
			base_center = v_add(cyl->center, v_scale(axis, cyl->attrs[CYLINDER_H_I]));
		else
			base_center = cyl->center;
		diff = v_sub(hit_point, base_center);
		if (fabsf(axis.x) > 0.1f)
			tangent = v_normalize(v_cross((t_vec3){0, 1, 0}, axis));
		else
			tangent = v_normalize(v_cross((t_vec3){1, 0, 0}, axis));
		bitangent = v_cross(axis, tangent);
		x_local = v_dot(diff, tangent);
		y_local = v_dot(diff, bitangent);
		radius = cyl->attrs[CYLINDER_D_I] / 2.0f;
		norm_rad = sqrtf(x_local * x_local + y_local * y_local) / radius;
		if (norm_rad > 1)
			norm_rad = 1;
		theta = 0.5f + (atan2f(y_local, x_local) / (2 * M_PI));
		cell_x_i = (int)(theta * CHECKER_X_COUNT);
		cell_y_i = (int)(norm_rad * 1);
		return ((cell_x_i + cell_y_i) % 2 == 1);
	}
	else
	{
		height = v_dot(diff, axis);
		local = v_sub(diff, v_scale(axis, height));
		if (fabsf(axis.x) > 0.1f)
			tangent = v_normalize(v_cross((t_vec3){0, 1, 0}, axis));
		else
			tangent = v_normalize(v_cross((t_vec3){1, 0, 0}, axis));
		bitangent = v_cross(axis, tangent);
		x_local = v_dot(local, tangent);
		y_local = v_dot(local, bitangent);
		theta = 0.5f + (atan2f(y_local, x_local) / (2 * M_PI));
		height = height / cyl->attrs[CYLINDER_H_I];
		if (height < 0)
			height = 0;
		if (height > 1)
			height = 1;
		cell_x_i = (int)(theta * CHECKER_X_COUNT);
		cell_y_i = (int)(height * CHECKER_Y_COUNT);
		return ((cell_x_i + cell_y_i) % 2 == 0);
	}
}
