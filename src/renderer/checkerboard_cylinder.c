/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard_cylinder.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:08:58 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/22 16:29:56 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include <math.h>

static void	init_xy_local(t_obj *cyl, float xy_local[2], t_vec3 vec)
{
	t_vec3	tangent;
	t_vec3	bitangent;

	if (fabsf(cyl->norm_vector.x) > 0.1f)
		tangent = v_normalize(v_cross((t_vec3){0, 1, 0}, cyl->norm_vector));
	else
		tangent = v_normalize(v_cross((t_vec3){1, 0, 0}, cyl->norm_vector));
	bitangent = v_cross(cyl->norm_vector, tangent);
	xy_local[0] = v_dot(vec, tangent);
	xy_local[1] = v_dot(vec, bitangent);
}

static bool	base(t_vec3 hit_point, t_obj *cyl, t_vec3 diff)
{
	float	xy_local[2];
	int		cell_xy_i[2];
	t_vec3	base_center;
	float	norm_rad;
	float	theta;

	base_center = cyl->center;
	if (v_dot(diff, cyl->norm_vector) > cyl->attrs[CYLINDER_H_I] * 0.5f)
		base_center = v_add(cyl->center, v_scale(cyl->norm_vector,
			cyl->attrs[CYLINDER_H_I]));
	diff = v_sub(hit_point, base_center);
	init_xy_local(cyl, xy_local, diff);
	theta = 0.5f + (atan2f(xy_local[1], xy_local[0]) / (2 * M_PI));
	norm_rad = sqrtf(xy_local[0] * xy_local[0] + xy_local[1] * xy_local[1]);
	norm_rad /= cyl->attrs[CYLINDER_D_I] / 2.0;
	norm_rad = clampf(norm_rad, norm_rad, 1);
	if (cyl->selected)
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT * 2);
	else
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT);
	cell_xy_i[1] = (int)(norm_rad * 1);
	return ((cell_xy_i[0] + cell_xy_i[1]) % 2);
}

static bool	surface(t_vec3 hit_point, t_obj *cyl, t_vec3 diff)
{
	float	xy_local[2];
	int		cell_xy_i[2];
	float	height;
	t_vec3	local;
	float	theta;

	height = v_dot(diff, cyl->norm_vector);
	local = v_sub(diff, v_scale(cyl->norm_vector, height));
	init_xy_local(cyl, xy_local, local);
	theta = 0.5f + (atan2f(xy_local[1], xy_local[0]) / (2 * M_PI));
	height = height / cyl->attrs[CYLINDER_H_I];
	height = clampf(height, 0, 1);
	if (cyl->selected)
	{
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT * 2);
		cell_xy_i[1] = (int)(height * CHECKER_Y_COUNT * 2);
	}
	else
	{
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT);
		cell_xy_i[1] = (int)(height * CHECKER_Y_COUNT);
	}
	return (!((cell_xy_i[0] + cell_xy_i[1]) % 2));
}

bool	cylinder_checkerboard(t_vec3 hit_point, t_obj *cyl, bool is_base)
{
	t_vec3	diff;

	diff = v_sub(hit_point, cyl->center);
	if (is_base)
		return (base(hit_point, cyl, diff));
	else
		return (surface(hit_point, cyl, diff));
}
