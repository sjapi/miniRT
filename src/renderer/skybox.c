/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:47:52 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/11 15:59:39 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include <math.h>

int draw_skybox(t_rt *info, t_ray *ray)
{
	t_scene	*scene;
	t_vec3	dir;
	float	u;
	float	v;
	int		sx;
	int		sy;
	char	*pixel;
	int		color;

	scene = info->scene;
	dir = ray->direction;
	u = 0.5f + atan2f(dir.z, dir.x) / (2.0f * M_PI);
	v = 0.5f - asinf(dir.y) / M_PI;
	if (u < 0)
		u += 1;
	if (u > 1)
		u -= 1;
	sx = (int)(u * scene->skybox_width);
	sy = (int)(v * scene->skybox_height);
	sx = clamp(sx, 0 scene->skybox_width - 1);
	sy = clamp(sy, 0 scene->skybox_height - 1);
	pixel = scene->skybox_data + sy * scene->skybox_line_length + sx * (scene->skybox_bpp / 8);
	color = *(int *)pixel;
	return (color);
}
