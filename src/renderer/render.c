/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:45:57 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/23 13:35:37 by haaghaja         ###   ########.fr       */
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
