/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:47:52 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/11 21:54:09 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"

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
	u = 0.5f + atan2f(dir.z, dir.x) * INV_2PI;
	v = 0.5f - asinf(dir.y) * INV_PI;
	if (u < 0)
		u += 1;
	if (u > 1)
		u -= 1;
	sx = (int)(u * scene->skybox->width);
	sy = (int)(v * scene->skybox->height);
	sx = clamp(sx, 0, scene->skybox->width - 1);
	sy = clamp(sy, 0, scene->skybox->height - 1);
	pixel = scene->skybox->data + sy * scene->skybox->line_length + sx * (scene->skybox->bpp / 8);
	color = *(int *)pixel;
	return (color);
}
