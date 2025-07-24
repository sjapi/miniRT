/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:36:01 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 16:16:43 by azolotar         ###   ########.fr       */
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
	t_optim	*optim;

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

static void	optimize_objs(t_rt *info)
{
	int		i;
	t_obj	*obj;
	t_vec3	apex;
	float	angle_rad;

	i = -1;
	while (++i < info->scene->objs_count)
	{
		obj = &info->scene->objs[i];
		if (obj->type == CONE)
		{
			apex = v_add(obj->center,
					v_scale(obj->norm_vector, obj->attrs[CONE_H_I]));
			obj->attrs[CONE_AP_X_I] = apex.x;
			obj->attrs[CONE_AP_Y_I] = apex.y;
			obj->attrs[CONE_AP_Z_I] = apex.z;
			angle_rad = obj->attrs[CONE_A_I] * M_PI / 180.0;
			obj->attrs[CONE_AR_I] = angle_rad;
			obj->attrs[CONE_TAN2] = tanf(angle_rad) * tanf(angle_rad);
			obj->attrs[CONE_COS2] = cosf(angle_rad) * cosf(angle_rad);
		}
		else if (obj->type == SPHERE)
			obj->attrs[SPHERE_R_I] = obj->attrs[SPHERE_D_I] * 0.5;
	}
}

static void	optimize_cam_basis(t_cam *cam)
{
	t_vec3	world_up;

	if (fabsf(cam->orient_v.x) < 1e-6 && fabsf(cam->orient_v.z) < 1e-6)
		world_up = (t_vec3){0, 0, 1};
	else
		world_up = (t_vec3){0, 1, 0};
	cam->right = v_normalize(v_cross(cam->orient_v, world_up));
	cam->up = v_cross(cam->right, cam->orient_v);
}

bool	init_optimization(t_rt *info)
{
	t_optim	*optim;

	if (info->optim == NULL)
	{
		optim = alloc_optim();
		if (!optim)
			return (false);
	}
	else
		optim = info->optim;
	optimize_viewport(info, optim);
	optimize_objs(info);
	optimize_cam_basis(info->scene->cam);
	info->optim = optim;
	return (true);
}
