/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:49:48 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/21 22:16:13 by haaghaja         ###   ########.fr       */
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
		v_scale(cam->up, ny)
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

/* MSAA */
static float   get_nx_sample(int x, float aspect_ratio, float tan_fov, float sample)
{
    float   nx;

    nx = (float)(x + sample) / (float)WIN_WIDTH;
    nx = (2.0 * nx - 1.0) * aspect_ratio * tan_fov;
    return (nx);
}

static float   get_ny_sample(int y, float tan_fov, float sample)
{
    float   ny;

    ny = (float)(y + sample) / (float)WIN_HEIGHT;
    ny = (1.0 - 2.0 * ny) * tan_fov;
    return (ny);
}

float	get_nx(int x, float aspect_ratio, float tan_fov)
{
	float	nx;

	nx = (float)(x + 0.5) / (float)WIN_WIDTH;
	nx = (2.0 * nx - 1.0) * aspect_ratio * tan_fov;
	return (nx);
}

float	get_ny(int y, float tan_fov)
{
	float	ny;

	ny = (float)(y + 0.5) / (float)WIN_HEIGHT;
	ny = (1.0 - 2.0 * ny) * tan_fov;
	return (ny);
}

void init_rays_msaa(t_ray rays[4], t_rt *info, int x, int y)
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
	//tan_fov = tanf((info->scene->cam->fov * M_PI / 180.0f) / 2.0f);
	i = 0;
	while (i < 4)
	{
		nx = get_nx_sample(x, info->win_aspect_ratio, info->scene->cam->t_fov, x_offsets[i % 2]);
		ny = get_ny_sample(y, info->scene->cam->t_fov, y_offsets[i % 2]);
		rays[i].direction = get_ray_dir(nx, ny, info->scene->cam);
		i++;
	}
}
