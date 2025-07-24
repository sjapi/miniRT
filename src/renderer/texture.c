/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:30:44 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 14:30:47 by haaghaja         ###   ########.fr       */
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

static float	get_bump_height(char *pixel)
{
	int	value;

	value = *(int *)pixel;
	return ((float)((value >> 16) & 0xFF) / 255.0f);
}

static void	get_bump_uv(t_texture *bump, float u, float v, int *coords)
{
	float	offset;
	float	u1;
	float	v1;

	offset = 1.0f / bump->width;
	u1 = u + offset;
	v1 = v + offset;
	coords[0] = clamp((int)(u * bump->width), 0, bump->width - 1);
	coords[1] = clamp((int)(v * bump->height), 0, bump->height - 1);
	coords[2] = clamp((int)(u1 * bump->width), 0, bump->width - 1);
	coords[3] = clamp((int)(v1 * bump->height), 0, bump->height - 1);
}

static void	get_bump_delta(t_texture *bump, int *c, float *dx, float *dy)
{
	char	*p00;
	char	*p10;
	char	*p01;
	float	h;

	p00 = bump->data + c[1] * bump->line_length + c[0] * (bump->bpp / 8);
	p10 = bump->data + c[1] * bump->line_length + c[2] * (bump->bpp / 8);
	p01 = bump->data + c[3] * bump->line_length + c[0] * (bump->bpp / 8);
	h = get_bump_height(p00);
	*dx = get_bump_height(p10) - h;
	*dy = get_bump_height(p01) - h;
}

void	apply_bump(t_hit *hit, float u, float v)
{
	t_vec3	tangent;
	t_vec3	bitangent;
	int		coords[4];
	float	dx;
	float	dy;

	get_bump_uv(hit->obj->texture, u, v, coords);
	get_bump_delta(hit->obj->texture, coords, &dx, &dy);
	tangent = v_normalize(v_cross((t_vec3){0, 1, 0}, hit->normal));
	if (v_len(tangent) == 0)
		tangent = (t_vec3){1, 0, 0};
	bitangent = v_normalize(v_cross(hit->normal, tangent));
	hit->normal = v_normalize(v_add(
				v_add(hit->normal, v_scale(tangent, dx)),
				v_scale(bitangent, dy)));
}

t_color	get_texture_color(t_hit *hit)
{
	if (hit->obj->type == SPHERE)
		return (get_sp_texture(hit));
	if (hit->obj->type == CYLINDER)
		return (get_cy_texture(hit));
	if (hit->obj->type == PLANE)
		return (get_pl_texture(hit));
	return ((t_color){0, 0, 0});
}
