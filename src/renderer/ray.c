/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:49:48 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/09 10:58:05 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include <math.h>

static t_point3	get_ray_dir(float nx, float ny, t_cam *cam)
{
	t_point3	forward;
	t_point3	world_up;
	t_point3	right;
	t_point3	up;
	t_point3	dir;

	forward = cam->orient_v;
	if (fabsf(forward.x) < 1e-6 && fabsf(forward.z) < 1e-6)
		world_up = (t_point3){0, 0, 1};
	else
		world_up = (t_point3){0, 1, 0};
	right = v_normalize(v_cross(world_up, forward));
	up = v_cross(forward, right);
	dir = v_add(
		v_add(forward, v_scale(right, nx)),
		v_scale(up, ny)
	);
	return (v_normalize(dir));
}

/*
 * at first nx, ny - normalized coordinates in range 0...1
 * and after they are converted to range -1...1 and scaled to actual size of viewport
 */
void	init_ray(t_ray *ray, t_rt *info, int x, int y)
{
	float		nx;
	float		ny;
	float		tan_fov;

	nx = (float)(x + 0.5) / (float)WIN_WIDTH;
	ny = (float)(y + 0.5) / (float)WIN_HEIGHT;
	tan_fov = tanf(info->scene->cam->fov / 2.0);
	nx = (2.0 * nx - 1.0) * info->win_aspect_ratio * tan_fov;
	ny = (1.0 - 2.0 * ny) * tan_fov;
	ray->origin = info->scene->cam->view_point;
	ray->direction = get_ray_dir(nx, ny, info->scene->cam);
}
