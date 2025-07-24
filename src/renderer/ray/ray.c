/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:49:48 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 02:15:20 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include <math.h>

t_vec3	get_ray_dir(float nx, float ny, t_cam *cam)
{
	t_vec3	dir;

	dir = v_add(
			v_add(cam->orient_v, v_scale(cam->right, nx)),
			v_scale(cam->up, ny));
	return (v_normalize(dir));
}

void	init_ray(t_ray *ray, t_rt *info, int x, int y)
{
	float		nx;
	float		ny;

	nx = info->optim->viewport_x[x];
	ny = info->optim->viewport_y[y];
	ray->direction = get_ray_dir(nx, ny, info->scene->cam);
}

void	init_rays_msaa(t_ray rays[4], t_rt *info, int x, int y)
{
	float	x_offsets[4];
	float	y_offsets[4];
	float	nx;
	float	ny;
	int		i;

	x_offsets[0] = 0.25f;
	x_offsets[1] = 0.75f;
	y_offsets[0] = 0.25f;
	y_offsets[1] = 0.25f;
	i = 0;
	while (i < 4)
	{
		nx = get_nx_sample(x, info->win_aspect_ratio, info->scene->cam->t_fov,
				x_offsets[i % 2]);
		ny = get_ny_sample(y, info->scene->cam->t_fov, y_offsets[i % 2]);
		rays[i].direction = get_ray_dir(nx, ny, info->scene->cam);
		i++;
	}
}
