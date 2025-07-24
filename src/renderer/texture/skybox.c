/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:47:52 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 14:35:23 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"

static void	skybox_uv(t_vec3 dir, float *u, float *v)
{
	*u = 0.5f + atan2f(dir.z, dir.x) * INV_2PI;
	*v = 0.5f - asinf(dir.y) * INV_PI;
	if (*u < 0)
		*u += 1;
	if (*u > 1)
		*u -= 1;
}

t_color	draw_skybox(t_rt *info, t_ray *ray)
{
	float	u;
	float	v;
	int		sx;
	int		sy;
	char	*pixel;

	skybox_uv(ray->direction, &u, &v);
	sx = clamp((int)(u * info->scene->skybox->width),
			0, info->scene->skybox->width - 1);
	sy = clamp((int)(v * info->scene->skybox->height),
			0, info->scene->skybox->height - 1);
	pixel = info->scene->skybox->data
		+ sy * info->scene->skybox->line_length
		+ sx * info->scene->skybox->bpp;
	return (int_to_color(*(int *)pixel));
}
