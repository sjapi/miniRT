/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard_cone.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:05:49 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/22 16:51:15 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include <math.h>

static int	base(t_obj *cone, t_vec3 diff, t_vec3 tangent, t_vec3 bitangent)
{
	float	radius_base;
	float	uv[2];
	float	theta;
	float	v_tex;
	int		cell_xy_i[2];

	radius_base = tanf(cone->attrs[CONE_AR_I]) * cone->attrs[CONE_H_I];
	uv[0] = v_dot(diff, tangent);
	uv[1] = v_dot(diff, bitangent);
	theta = 0.5f + atan2f(uv[1], uv[0]) / (2 * M_PI);
	v_tex = v_len((t_vec3){uv[0], uv[1], 0}) / radius_base;
	v_tex = clampf(v_tex, v_tex, 1);
	if (cone->selected)
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT * 2);
	else
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT);
	cell_xy_i[1] = (int)(v_tex * 1);
	return ((cell_xy_i[0] + cell_xy_i[1]) % 2);
}

static int	surface(t_obj *cone, t_vec3 diff, t_vec3 tangent, t_vec3 bitangent)
{
	t_vec3	radial;
	float	uv[2];
	float	theta;
	float	v_tex;
	int		cell_xy_i[2];

	radial = v_sub(diff, v_scale(cone->norm_vector,
				v_dot(diff, cone->norm_vector)));
	uv[0] = v_dot(radial, tangent);
	uv[1] = v_dot(radial, bitangent);
	theta = 0.5f + atan2f(uv[1], uv[0]) / (2 * M_PI);
	v_tex = (cone->attrs[CONE_H_I] - v_dot(diff, cone->norm_vector));
	v_tex /= cone->attrs[CONE_H_I];
	v_tex = clampf(v_tex, 0, 1);
	if (cone->selected)
	{
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT * 2);
		cell_xy_i[1] = (int)(v_tex * CHECKER_Y_COUNT * 2);
	}
	else
	{
		cell_xy_i[0] = (int)(theta * CHECKER_X_COUNT);
		cell_xy_i[1] = (int)(v_tex * CHECKER_Y_COUNT);
	}
	return (!((cell_xy_i[0] + cell_xy_i[1]) % 2));
}

bool	cone_checkerboard(t_vec3 hit_point, t_obj *cone, bool in_base)
{
	t_vec3	diff;
	t_vec3	tangent;
	t_vec3	bitangent;

	if (fabsf(cone->norm_vector.x) > 0.1f)
		tangent = v_normalize(v_cross((t_vec3){0, 1, 0}, cone->norm_vector));
	else
		tangent = v_normalize(v_cross((t_vec3){1, 0, 0}, cone->norm_vector));
	bitangent = v_cross(cone->norm_vector, tangent);
	diff = v_sub(hit_point, cone->center);
	if (in_base)
		return (base(cone, diff, tangent, bitangent));
	else
		return (surface(cone, diff, tangent, bitangent));
}
