/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:36:01 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/11 16:43:47 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"
#include "utils.h"
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
		free(optim->viewport_x);
		free(optim);
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

static void	optimize_viewport(t_rt *info, t_optim *optim)
{
	float	tan_fov;
	int		x;
	int		y;

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
}

static void	optimize_cone(t_obj *cone)
{
	t_vec3	apex;

	apex = v_add(cone->center, v_scale(cone->norm_vector, cone->attrs[CONE_H_I]));
	cone->attrs[CONE_AP_X_I] = apex.x;
	cone->attrs[CONE_AP_Y_I] = apex.y;
	cone->attrs[CONE_AP_Z_I] = apex.z;
	cone->attrs[CONE_AR_I] = cone->attrs[CONE_A_I] * M_PI / 180.0;
}

static void	optimize_cylinder(t_obj *cylinder)
{
	(void)cylinder;
}

static void	optimize_objs(t_rt *info)
{
	int	i;

	i = 0;
	while (i < info->scene->objs_count)
	{
		if (info->scene->objs[i].type == CONE)
			optimize_cone(&info->scene->objs[i]);
		else if (info->scene->objs[i].type == CYLINDER)
			optimize_cylinder(&info->scene->objs[i]);
		i++;
	}
}

bool	init_optimization(t_rt *info)
{
	t_optim *optim;

	optim = alloc_optim();
	if (!optim)
		return (false);
	optimize_viewport(info, optim);
	optimize_objs(info);
	info->optim = optim;
	return (true);
}
