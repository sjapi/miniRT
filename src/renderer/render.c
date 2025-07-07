/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:38:47 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/07 18:53:59 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include "mlx.h"
#include <stdio.h>

void	render_scene(t_rt *info)
{
	int	i = 0;
	int	j;
	int	amb = get_amb_color(info->scene->amb);

	while (i < WIN_HEIGHT)
	{
		j = 0;
		while (j < WIN_WIDTH)
		{
			img_put_pixel_safe(info, j, i, amb);
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
}
