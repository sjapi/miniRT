/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:43:55 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/07 15:55:15 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"

void	img_put_pixel_safe(t_rt *info, int x, int y, int color)
{
	char	*pixel;

	if (x < 0 || x >= WIN_WIDTH)
		return ;
	if (y < 0 || y >= WIN_HEIGHT)
		return ;
	pixel = info->addr + (info->line_len * y + x * (info->bpp / 8));
	*(int *)pixel = color;
}
