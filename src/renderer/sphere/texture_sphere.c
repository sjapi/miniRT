/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 00:26:55 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/24 00:35:06 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "minirt.h"
#include "defines.h"
#include "renderer.h"

static void	sp_sphere_uv(t_hit *hit, float *u, float *v)
{
	t_vec3	p;
	float	radius;

	p = v_sub(hit->hit_point, hit->obj->center);
	radius = hit->obj->attrs[SPHERE_D_I] * 0.5;
	*u = 0.5 - atan2(p.x, p.z) * INV_2PI;
	*v = 0.5 - asin(clampf(p.y / radius, -1, 1)) * INV_PI;
}

static t_color	sp_get_pixel(t_texture *t, float u, float v)
{
	int		sx;
	int		sy;
	char	*pixel;

	sx = clamp((int)(u * (t->width - 1)), 0, t->width - 1);
	sy = clamp((int)(v * (t->height - 1)), 0, t->height - 1);
	pixel = t->data + sy * t->line_length + sx * (t->bpp / 8);
	return (int_to_color(*(int *)pixel));
}

t_color	get_sp_texture(t_hit *hit)
{
	float		u;
	float		v;
	t_texture	*t;
	t_color		col;

	t = hit->obj->texture;
	sp_sphere_uv(hit, &u, &v);
	col = sp_get_pixel(t, u, v);
	apply_bump(hit, u, v);
	return (col);
}
