/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:49:48 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/09 19:43:03 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include <math.h>

static t_vec3	get_ray_dir(float nx, float ny, t_cam *cam)
{
	t_vec3	forward;
	t_vec3	world_up;
	t_vec3	right;
	t_vec3	up;
	t_vec3	dir;

	forward = cam->orient_v;
	if (fabsf(forward.x) < 1e-6 && fabsf(forward.z) < 1e-6)
		world_up = (t_vec3){0, 0, 1};
	else
		world_up = (t_vec3){0, 1, 0};
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

	nx = info->optim->viewport_x[x];
	ny = info->optim->viewport_y[y];
	ray->direction = get_ray_dir(nx, ny, info->scene->cam);
}
