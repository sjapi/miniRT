/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:43:55 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 14:59:18 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"
#include "utils.h"
#include <math.h>

void	img_put_pixel_safe(t_rt *info, int x, int y, int color)
{
	char	*pixel;

	if (x < 0 || x > WIN_WIDTH)
		return ;
	if (y < 0 || y > WIN_HEIGHT)
		return ;
	pixel = info->addr + (info->line_len * y + x * (info->bpp / 8));
	*(int *)pixel = color;
}

static void	init_vals(float f[4], int i[6], t_vec3 a, t_vec3 b)
{
	i[2] = a.x;
	i[3] = a.y;
	i[4] = b.x;
	i[5] = b.y;
	f[0] = ft_abs(i[4] - i[2]);
	f[1] = -ft_abs(i[5] - i[3]);
	i[0] = -1;
	if (i[2] < i[4])
		i[0] = 1;
	i[1] = -1;
	if (i[3] < i[5])
		i[1] = 1;
	f[2] = f[0] + f[1];
}

void	img_draw_line(t_rt *info, t_vec3 a, t_vec3 b, int color)
{
	float	f[4];
	int		i[6];

	init_vals(f, i, a, b);
	while (1)
	{
		img_put_pixel_safe(info, i[2], i[3], color);
		if (i[2] == i[4] && i[3] == i[5])
			break ;
		f[3] = 2 * f[2];
		if (f[3] >= f[1])
		{
			f[2] += f[1];
			i[2] += i[0];
		}
		if (f[3] <= f[0])
		{
			f[2] += f[0];
			i[3] += i[1];
		}
	}
}
