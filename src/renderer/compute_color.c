/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:10:39 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/21 17:58:08 by azolotar         ###   ########.fr       */
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

static	t_color	compute_object_color(t_hit *hit)
{
	float u, v;
	int sx, sy;
	char *pixel;
	t_color col;

	if (!hit->obj->texture)
		return (int_to_color(hit->obj->color));
	t_vec3 p = v_sub(hit->hit_point, hit->obj->center);
	float radius = hit->obj->attrs[SPHERE_D_I];

	u = 0.5 + atan2(p.z, p.x) * INV_2PI;
	v = 0.5 - asin(p.y / radius) * INV_PI;

	sx = clamp((int)(u * hit->obj->texture->width), 0, hit->obj->texture->width - 1);
	sy = clamp((int)(v * hit->obj->texture->height), 0, hit->obj->texture->height - 1);

	pixel = hit->obj->texture->data + sy * hit->obj->texture->line_length + sx * (hit->obj->texture->bpp / 8);
	int color_int = *(int *)pixel;
	return int_to_color(color_int);
}

static t_color	compute_shadow_ray(t_light *light, t_hit *primary_hit, t_rt *info);

static t_color	compute_mirror(t_light *light, t_hit *primary_hit, t_rt *info)
{
	t_ray	mirror_ray;
	t_hit	mirror_hit;

	mirror_ray.origin = v_add(primary_hit->hit_point, v_scale(primary_hit->normal, 1e-4));
	mirror_ray.direction = primary_hit->normal; if (find_hit(&mirror_ray, info, &mirror_hit, false))
	{
		return (compute_shadow_ray(light, &mirror_hit, info));
	}
	if (info->scene->skybox)
		return (draw_skybox(info, &mirror_ray));
	else
		return ((t_color){0, 0, 0});
}

static t_color	compute_shadow_ray(t_light *light, t_hit *primary_hit, t_rt *info)
{
	t_ray	shadow_ray;
	t_hit	shadow_hit;
	t_color	final = {0, 0, 0};
	float	light_dist;
	bool	in_shadow;

	if (primary_hit->obj->mirror)
		return (decrease_color(compute_mirror(light, primary_hit, info), 30));
	t_color	obj_col = compute_object_color(primary_hit);
	if (primary_hit->reverse)
	{
		obj_col.r = 255.0 - obj_col.r;
		obj_col.g = 255.0 - obj_col.g;
		obj_col.b = 255.0 - obj_col.b;
	}
	t_color	amb_col = int_to_color(get_amb_color(info->scene->amb));
	t_color light_col = int_to_color(light->color);
	shadow_ray.origin = v_add(primary_hit->hit_point, v_scale(primary_hit->normal, 1e-4));
	shadow_ray.direction = v_sub(light->point, primary_hit->hit_point);
	shadow_ray.direction = v_normalize(shadow_ray.direction);
	light_dist = v_len(v_sub(light->point, primary_hit->hit_point));
	in_shadow = false;
	if (find_hit(&shadow_ray, info, &shadow_hit, true))
	{
		if (shadow_hit.t > 1e-3 && shadow_hit.t < light_dist)
			in_shadow = true;
	}
	// Compute diffuse
	if (!in_shadow)
	{
		compute_diffuse(&final, primary_hit, &shadow_ray, light, &obj_col);
		compute_specular(&final, primary_hit, &shadow_ray, light, info->scene->cam);
	}
	// Ambient * object
	t_color ambient_col = {
		obj_col.r * amb_col.r / 255.0f * info->scene->amb->ratio,
		obj_col.g * amb_col.g / 255.0f * info->scene->amb->ratio,
		obj_col.b * amb_col.b / 255.0f * info->scene->amb->ratio
	};
	final = color_clamp(color_add(final, ambient_col));
	return (final);
}

t_color	compute_color(t_hit *p_hit, t_rt *info)
{
	int		i;
	t_color	shadow_color;
	t_color	total_color;

	ft_bzero(&total_color, sizeof(t_color));
	i = 0;
	while (i < info->scene->lights_count)
	{
		shadow_color = compute_shadow_ray(&info->scene->lights[i], p_hit, info);
		total_color = color_add(total_color, shadow_color);
		i++;
	}
	return (color_clamp(total_color));
}
