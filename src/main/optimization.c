/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:36:01 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/09 20:03:08 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

static t_optim	*alloc_optim(void)
{
	t_optim *optim;

	optim = malloc(sizeof(t_optim));
	if (!optim)
		return (NULL);
	optim->viewport_x = malloc(sizeof(float) * WIN_WIDTH);
	if (!optim->viewport_x)
	{
		free(optim);
		return (NULL);
	}
	optim->viewport_y = malloc(sizeof(float) * WIN_HEIGHT);
	if (!optim->viewport_y)
	{
		free(optim);
		free(optim->viewport_x);
		return (NULL);
	}
	return (optim);
}

static float	get_nx(int x, float aspect_ratio, float tan_fov)
{
	float	nx;

	nx = (float)(x + 0.5) / (float)WIN_WIDTH;
	nx = (2.0 * nx - 1.0) * aspect_ratio * tan_fov;
	return (nx);
}

static float	get_ny(int y, float tan_fov)
{
	float	ny;

	ny = (float)(y + 0.5) / (float)WIN_HEIGHT;
	ny = (1.0 - 2.0 * ny) * tan_fov;
	return (ny);
}

bool	init_optimization(t_rt *info)
{
	t_optim *optim;
	int		x;
	int		y;
	float	tan_fov;

	optim = alloc_optim();
	if (!optim)
		return (false);
	tan_fov = tanf((info->scene->cam->fov * M_PI / 180.0) / 2.0);
	x = 0;
	while (x < WIN_WIDTH)
	{
		optim->viewport_x[x] = get_nx(x, info->win_aspect_ratio, tan_fov);
		x++;
	}
	y = 0;
	while (y < WIN_HEIGHT)
	{
		optim->viewport_y[y] = get_ny(y, tan_fov);
		y++;
	}
	info->optim = optim;
	return (true);
}
