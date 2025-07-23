/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:45:57 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 01:23:01 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

static t_color	manage_hit(t_hit *hit, t_ray *ray, t_rt *info)
{
	t_color	result;

	ft_bzero(hit, sizeof(t_hit));
	ray->origin = info->scene->cam->viewpoint;
	if (find_hit(ray, info->scene, hit))
		result = compute_color(hit, info);
	else if (info->scene->skybox)
		result = draw_skybox(info, ray);
	else
		result = (t_color){0, 0, 0};
	return (result);
}

static int	render_pixel_full(int x, int y, t_rt *info)
{
	t_ray	rays[4];
	t_hit	hit;
	t_color	result;
	t_color	color;
	int		i;

	ft_bzero(&result, sizeof(t_color));
	init_rays_msaa(rays, info, x, y);
	i = -1;
	while (++i < 4)
	{
		color = manage_hit(&hit, &rays[i], info);
		result = color_add(result, color);
	}
	result = color_clamp(color_avg(result, 4));
	img_put_pixel_safe(info, x, y, color_to_int(result));
	return (1);
}

static int	render_pixel_light(int x, int y, t_rt *info)
{
	t_ray	ray;
	t_hit	hit;
	t_color	result;
	int		i;

	init_ray(&ray, info, x, y);
	result = manage_hit(&hit, &ray, info);
	img_put_pixel_safe(info, x, y, color_to_int(result));
	img_put_pixel_safe(info, x + 1, y, color_to_int(result));
	img_put_pixel_safe(info, x, y + 1, color_to_int(result));
	img_put_pixel_safe(info, x + 1, y + 1, color_to_int(result));
	return (2);
}

//=====================================
static int	project_point(t_cam *cam, t_vec3 p, int *sx, int *sy)
{
	t_vec3 rel = v_sub(p, cam->viewpoint);
	float cx = v_dot(rel, cam->right);
	float cy = v_dot(rel, cam->up);
	float cz = v_dot(rel, cam->orient_v);
	if (cz <= 0.01f) // behind camera
		return (0);
	float focal = (WIN_WIDTH / 2.0f) / tanf(cam->t_fov * 0.5f);
	*sx = (int)((cx / cz) * focal + WIN_WIDTH / 2.0f);
	*sy = (int)((cy / cz) * focal + WIN_HEIGHT / 2.0f);
	return (1);
}

void	draw_bb(t_rt *info, t_obj *obj, int color)
{
	t_vec3 min = obj->aabb_min;
	t_vec3 max = obj->aabb_max;
	t_vec3 p[8] = {
		{min.x, min.y, min.z}, {max.x, min.y, min.z},
		{max.x, max.y, min.z}, {min.x, max.y, min.z},
		{min.x, min.y, max.z}, {max.x, min.y, max.z},
		{max.x, max.y, max.z}, {min.x, max.y, max.z}
	};
	int edges[12][2] = {
		{0,1},{1,2},{2,3},{3,0}, // bottom
		{4,5},{5,6},{6,7},{7,4}, // top
		{0,4},{1,5},{2,6},{3,7}  // vertical
	};
	int proj[8][2];
	int i;

	// Project points
	for (i = 0; i < 8; i++)
	{
		if (!project_point(info->scene->cam, p[i], &proj[i][0], &proj[i][1]))
			return; // skip drawing if any point behind camera
	}

	// Draw edges
	for (i = 0; i < 12; i++)
	{
		t_vec3 a = {proj[edges[i][0]][0], proj[edges[i][0]][1], 0};
		t_vec3 b = {proj[edges[i][1]][0], proj[edges[i][1]][1], 0};
		img_draw_line(info, a, b, color);
	}
}
//===================================


static void	render_scene(t_rt *info)
{
	int		x;
	int		y;
	int		step;

	step = (info->mode != RENDER_MODE) + 1;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			if (info->mode == RENDER_MODE)
			{
				step = render_pixel_full(x, y, info);
			}
			else
			{
				step = render_pixel_light(x, y, info);
			}
			x += step;
		}
		y += step;
	}
	if (info->scene->selected)
		draw_bb(info, info->scene->selected, 0x00FF00);
}

void	render(t_rt *info)
{
	long	time;

	time = current_time();

	init_optimization(info);
	render_scene(info);
	draw_xyz_axis(info);
	printf("Render time: %ld\n", current_time() - time);

	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
	draw_info(info);
}
