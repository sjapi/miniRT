/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:30:44 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 00:57:22 by haaghaja         ###   ########.fr       */
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
	int value = *(int *)pixel;
	return ((float)((value >> 16) & 0xFF) / 255.0f);
}

void	apply_bump(t_hit *hit, float u, float v)
{
	t_texture	*bump = hit->obj->texture;
	float		radius = hit->obj->attrs[SPHERE_D_I] * 0.5f;
	float		offset = 1.0f / bump->width;

	// UV сдвиги
	float u1 = u + offset;
	float v1 = v + offset;

	int sx = clamp((int)(u * bump->width), 0, bump->width - 1);
	int sy = clamp((int)(v * bump->height), 0, bump->height - 1);
	int sx1 = clamp((int)(u1 * bump->width), 0, bump->width - 1);
	int sy1 = clamp((int)(v1 * bump->height), 0, bump->height - 1);

	char *p00 = bump->data + sy * bump->line_length + sx * (bump->bpp / 8);
	char *p10 = bump->data + sy * bump->line_length + sx1 * (bump->bpp / 8);
	char *p01 = bump->data + sy1 * bump->line_length + sx * (bump->bpp / 8);

	float h = get_bump_height(p00);
	float hx = get_bump_height(p10);
	float hy = get_bump_height(p01);

	float dx = hx - h;
	float dy = hy - h;

	// Построение tangent-базы
	t_vec3 up = {0, 1, 0};
	t_vec3 tangent = v_normalize(v_cross(up, hit->normal));
	if (v_len(tangent) == 0)
		tangent = (t_vec3){1, 0, 0};
	t_vec3 bitangent = v_normalize(v_cross(hit->normal, tangent));

	t_vec3 bump_normal = hit->normal;
	bump_normal = v_add(bump_normal, v_scale(tangent, dx));
	bump_normal = v_add(bump_normal, v_scale(bitangent, dy));
	hit->normal = v_normalize(bump_normal);
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
