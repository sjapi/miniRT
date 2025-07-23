/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 00:57:03 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/24 01:00:57 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "minirt.h"
#include "defines.h"
#include "renderer.h"

static void	pl_plane_uv(t_hit *hit, float *u, float *v)
{
	t_vec3	rel;
	t_vec3	u_axis;
	t_vec3	v_axis;

	if (fabsf(hit->obj->norm_vector.y) < 0.999f)
		u_axis = v_normalize(v_cross((t_vec3){0, 1, 0}, hit->obj->norm_vector));
	else
		u_axis = v_normalize(v_cross((t_vec3){1, 0, 0}, hit->obj->norm_vector));
	v_axis = v_cross(hit->obj->norm_vector, u_axis);
	rel = v_sub(hit->hit_point, hit->obj->center);
	*u = fmodf(v_dot(rel, u_axis), 1.0f);
	if (*u < 0)
		*u += 1.0f;
	*v = fmodf(v_dot(rel, v_axis), 1.0f);
	if (*v < 0)
		*v += 1.0f;
}

static t_color	pl_get_pixel(t_texture *t, float u, float v)
{
	int		sx;
	int		sy;
	char	*pixel;

	sx = clamp((int)(u * (t->width - 1)), 0, t->width - 1);
	sy = clamp((int)((1.0f - v) * (t->height - 1)), 0, t->height - 1);
	pixel = t->data + sy * t->line_length + sx * (t->bpp / 8);
	return (int_to_color(*(int *)pixel));
}

t_color	get_pl_texture(t_hit *hit)
{
	float		u;
	float		v;
	t_texture	*t;
	t_color		col;

	t = hit->obj->texture;
	pl_plane_uv(hit, &u, &v);
	col = pl_get_pixel(t, u, v);
	return (col);
}
