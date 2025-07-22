/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard_sphere.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:02:45 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/22 15:13:23 by azolotar         ###   ########.fr       */
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

	normal = v_normalize(v_sub(hit_point, sphere->center));
	longitude = 0.5f + (atan2f(normal.z, normal.x) / (2 * M_PI));
	latitude = 0.5f - (asinf(normal.y) / M_PI);
	if (sphere->selected)
	{
		cell_x_i = (int)(longitude * CHECKER_X_COUNT * 2);
		cell_y_i = (int)(latitude * CHECKER_Y_COUNT * 2);
	}
	else
	{
		cell_x_i = (int)(longitude * CHECKER_X_COUNT);
		cell_y_i = (int)(latitude * CHECKER_Y_COUNT);
	}
	return ((cell_x_i + cell_y_i) % 2 == 0);
}
