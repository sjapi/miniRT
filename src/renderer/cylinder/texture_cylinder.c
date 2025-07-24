/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_cylinder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 00:35:53 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/24 18:27:37 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "minirt.h"
#include "renderer.h"
#include "defines.h"

static void	cy_cylinder_uv(t_hit *hit, float *u, float *v)
{
	t_vec3	p;
	t_vec3	axis_p;
	float	height;
	float	proj;

	height = hit->obj->attrs[CYLINDER_H_I];
	p = v_sub(hit->hit_point, hit->obj->center);
	proj = v_dot(p, hit->obj->norm_vector);
	proj = clampf(proj, 0.0f, height);
	axis_p = v_sub(p, v_scale(hit->obj->norm_vector, proj));
	*u = 0.5f - atan2(axis_p.x, axis_p.z) * INV_2PI;
	*v = proj / height;
}

static t_color	cy_get_pixel(t_texture *t, float u, float v)
{
	int		sx;
	int		sy;
	char	*pixel;

	sx = clamp((int)(u * (t->width - 1)), 0, t->width - 1);
	sy = clamp((int)((1.0f - v) * (t->height - 1)), 0, t->height - 1);
	pixel = t->data + sy * t->line_length + sx * (t->bpp / 8);
	return (int_to_color(*(int *)pixel));
}

t_color	get_cy_texture(t_hit *hit)
{
	float		u;
	float		v;
	t_texture	*t;
	t_color		col;

	t = hit->obj->texture;
	cy_cylinder_uv(hit, &u, &v);
	col = cy_get_pixel(t, u, v);
	apply_bump(hit, u, v);
	return (col);
}
