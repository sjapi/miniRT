/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:49:48 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/11 17:36:38 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include <math.h>

static t_vec3	get_ray_dir(float nx, float ny, t_cam *cam)
{
	t_vec3	world_up;
	t_vec3	right;
	t_vec3	up;
	t_vec3	dir;

	if (fabsf(cam->orient_v.x) < 1e-6 && fabsf(cam->orient_v.z) < 1e-6)
		world_up = (t_vec3){0, 0, 1};
	else
		world_up = (t_vec3){0, 1, 0};
	//t_vec3 test = v_cross(world_up, cam->orient_v);
	//printf("%f %f %f\n", test.x, test.y, test.z);
	right = v_normalize(v_cross(world_up, cam->orient_v));
	up = v_cross(cam->orient_v, right);
	dir = v_add(
		v_add(cam->orient_v, v_scale(right, nx)),
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
