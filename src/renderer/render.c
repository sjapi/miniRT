/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:38:47 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/09 10:57:38 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>

void	render_scene(t_rt *info)
{
	int		y = 0;
	int		x;
	int		amb = get_amb_color(info->scene->amb);
	t_ray	ray;
	t_hit	hit;

	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			init_ray(&ray, info, x, y);
			if (find_hit(&ray, info, &hit))
			{
				int color = get_hit_color(&hit, info->scene);
				img_put_pixel_safe(info, x, y, color);
			}
			else
			{
				img_put_pixel_safe(info, x, y, amb);
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
}
