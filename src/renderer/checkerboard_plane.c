/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard_plane.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:03:58 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/22 15:16:46 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include <math.h>

bool	plane_checkerboard(t_vec3 hit_point, t_obj *plane)
{
	t_vec3	local;
	t_vec3	ortog_x;
	t_vec3	ortog_y;
	float	uv[2];
	int		xy[2];

	local = v_sub(hit_point, plane->center);
	ortog_x = v_cross((t_vec3){0.0f, 1.0f, 0.0f}, plane->norm_vector);
	if (v_len(ortog_x) < 1e-3f)
		ortog_x = v_cross((t_vec3){0.0f, 0.0f, 1.0f}, plane->norm_vector);
	ortog_x = v_normalize(ortog_x);
	ortog_y = v_normalize(v_cross(plane->norm_vector, ortog_x));
	uv[0] = v_dot(local, ortog_x);
	uv[1] = v_dot(local, ortog_y);
	if (plane->selected)
	{
		xy[0] = (int)floorf(uv[0] / (CHECKER_CELL_SIZE * 2));
		xy[1] = (int)floorf(uv[1] / (CHECKER_CELL_SIZE * 2));
	}
	else
	{
		xy[0] = (int)floorf(uv[0] / CHECKER_CELL_SIZE);
		xy[1] = (int)floorf(uv[1] / CHECKER_CELL_SIZE);
	}
	return (!((xy[0] + xy[1]) % 2));
}
