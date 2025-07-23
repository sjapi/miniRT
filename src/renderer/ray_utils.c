/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:11:55 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/24 02:14:01 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"

/* MSAA */
float	get_nx_sample(int x, float ratio, float tan_fov, float sample)
{
	float	nx;

	nx = (float)(x + sample) / (float)WIN_WIDTH;
	nx = (2.0 * nx - 1.0) * ratio * tan_fov;
	return (nx);
}

float	get_ny_sample(int y, float tan_fov, float sample)
{
	float	ny;

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
