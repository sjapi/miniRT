/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:12:04 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/20 13:14:32 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_color	int_to_color(int icolor)
{
	t_color	color;

	color.r = (icolor >> 16) & 0xff;
	color.g = (icolor >> 8) & 0xff;
	color.b = icolor & 0xff;
	return (color);
}

int	color_to_int(t_color color)
{
	return ((color.r & 0xFF) << 16) | ((color.g & 0xFF) << 8) | (color.b & 0xFF);
}

t_color	color_add(t_color a, t_color b)
{
	t_color	res;

	res.r = a.r + b.r;
	res.g = a.g + b.g;
	res.b = a.b + b.b;
	return (res);
}

t_color	color_clamp(t_color c)
{
	c.r = clampf(c.r, 0, 255);
	c.g = clampf(c.g, 0, 255);
	c.b = clampf(c.b, 0, 255);
	return c;
}

t_color	color_avg(t_color c, int count)
{
	c.r /= count;
	c.g /= count;
	c.b /= count;
	return (c);
}
