/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:10:39 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/23 15:39:35 by haaghaja         ###   ########.fr       */
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

static void	apply_bump(t_hit *hit, float u, float v)
{
	t_texture	*bump = hit->obj->texture;
	float		radius = hit->obj->attrs[SPHERE_D_I] * 0.5f;
	float		offset = 1.0f / bump->width;

	// UV сдвиги
	float u1 = u + offset;
	float v1 = v + offset;

	int sx  = clamp((int)(u * bump->width), 0, bump->width - 1);
	int sy  = clamp((int)(v * bump->height), 0, bump->height - 1);
	int sx1 = clamp((int)(u1 * bump->width), 0, bump->width - 1);
	int sy1 = clamp((int)(v1 * bump->height), 0, bump->height - 1);

	char *p00 = bump->data + sy  * bump->line_length + sx  * (bump->bpp / 8);
	char *p10 = bump->data + sy  * bump->line_length + sx1 * (bump->bpp / 8);
	char *p01 = bump->data + sy1 * bump->line_length + sx  * (bump->bpp / 8);

	float h  = get_bump_height(p00);
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

static t_color	get_texture_color(t_hit *hit)
{
	float		u;
	float		v;
	int			sx;
	int			sy;
	char		*pixel;
	t_texture	*t;
	t_vec3		p;
	t_color		col;
	float		radius;

	t = hit->obj->texture;
	p = v_sub(hit->hit_point, hit->obj->center);
	radius = hit->obj->attrs[SPHERE_D_I] * 0.5;

	u = 0.5 - atan2(p.x, p.z) * INV_2PI;
	v = 0.5 - asin(clampf(p.y / radius, -1, 1)) * INV_PI;

	sx = clamp((int)(u * (t->width - 1)), 0, t->width - 1);
	sy = clamp((int)(v * (t->height - 1)), 0, t->height - 1);
	pixel = t->data + sy * t->line_length + sx * (t->bpp / 8);
	col = int_to_color(*(int *)pixel);
	apply_bump(hit, u, v);
	return (col);
// 	uncomment it if you want texture without its color
//	return ((t_color){211,211,211});
}

static t_color	get_obj_color(t_hit *hit)
{
	t_color	color;

	if (hit->obj->texture == NULL)
		color = int_to_color(hit->obj->color);
	else
		color = get_texture_color(hit);
	if (hit->reverse)
		return (reverse_color(color));
	return (color);
}

static t_color	compute_amb_color(t_color obj_color, t_amb_light *amb)
{
	t_color	amb_color;
	
	amb_color = int_to_color(get_amb_color(amb));
	amb_color.r = obj_color.r * amb_color.r / 255.0f * amb->ratio;
	amb_color.g = obj_color.g * amb_color.g / 255.0f * amb->ratio;
	amb_color.b = obj_color.b * amb_color.b / 255.0f * amb->ratio;
	return (amb_color);
}

static t_color	compute_shadow_ray(t_color obj_col, t_light *light,
					t_hit *p_hit, t_rt *info);

static t_color	compute_mirror(t_color obj_col, t_light *light,
					t_hit *p_hit, t_rt *info)
{
	t_ray	ray;
	t_hit	hit;

	ray.origin = v_add(p_hit->hit_point, v_scale(p_hit->normal, 1e-4));
	ray.direction = p_hit->normal;
	if (find_hit(&ray, info->scene, &hit))
	{
		obj_col = get_obj_color(&hit);
		return (compute_shadow_ray(obj_col, light, &hit, info));
	}
	if (info->scene->skybox)
		return (draw_skybox(info, &ray));
	return ((t_color){0, 0, 0});
}

static t_color	compute_shadow_ray(t_color obj_col, t_light *light,
					t_hit *p_hit, t_rt *info)
{
	t_ray	shadow_ray;
	t_vec3	light_vec;
	t_color	final;

	if (p_hit->obj->mirror)
		return ((compute_mirror(obj_col, light, p_hit, info)));
	ft_bzero(&final, sizeof(t_color));
	light_vec = v_sub(light->point, p_hit->hit_point);
	shadow_ray.origin = p_hit->hit_point;
	shadow_ray.direction = v_normalize(light_vec);
	//final.r = 255;
	if (is_in_shadow(&shadow_ray, info->scene, v_len(light_vec)))
		return (final);
	final.r = 0;
	compute_diffuse(&final, p_hit, &shadow_ray, light, &obj_col);
	compute_specular(&final, p_hit, &shadow_ray, light, info->scene->cam);
	return (final);
}

/* main func */
t_color	compute_color(t_hit *p_hit, t_rt *info)
{
	int		i;
	t_color	shadow_color;
	t_color	total_color;
	t_color	obj_color;
	t_color	amb_color;

	obj_color = get_obj_color(p_hit);
	amb_color = compute_amb_color(obj_color, info->scene->amb);
	ft_bzero(&total_color, sizeof(t_color));
	i = -1;
	while (++i < info->scene->lights_count)
	{
		shadow_color = compute_shadow_ray(
				obj_color, &info->scene->lights[i], p_hit, info);
		total_color = color_add(total_color, shadow_color);
	}
	if (!p_hit->obj->mirror)
		total_color = color_clamp(color_add(total_color, amb_color));
	return (color_clamp(total_color));
}
