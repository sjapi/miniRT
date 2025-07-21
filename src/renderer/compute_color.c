/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:10:39 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/21 20:57:49 by azolotar         ###   ########.fr       */
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

static void	init_shadow_ray(t_ray *shadow_ray, t_hit *p_hit, t_light *light)
{
	shadow_ray->origin = p_hit->hit_point;//v_add(p_hit->hit_point, v_scale(p_hit->normal, 1e-4));
	shadow_ray->direction = v_normalize(v_sub(light->point, p_hit->hit_point));
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

	t = hit->obj->texture;
	p = v_sub(hit->hit_point, hit->obj->center);
	u = 0.5 + atan2(p.z, p.x) * INV_2PI;
	v = 0.5 - asin(p.y / hit->obj->attrs[SPHERE_D_I]) * INV_PI;
	sx = clamp((int)(u * t->width), 0, t->width - 1);
	sy = clamp((int)(v * t->height), 0, t->height - 1);
	pixel = t->data + sy * t->line_length + sx * (t->bpp / 8);
	col = int_to_color(*(int *)pixel);
	return (col);
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

static t_color	compute_shadow_ray(t_color obj_col, t_light *light, t_hit *primary_hit, t_rt *info);

static t_color	compute_mirror(t_color obj_col, t_light *light, t_hit *primary_hit, t_rt *info)
{
	t_ray	ray;
	t_hit	hit;

	ray.origin = v_add(primary_hit->hit_point, v_scale(primary_hit->normal, 1e-4));
	ray.direction = primary_hit->normal;
	if (find_hit(&ray, info, &hit, false))
	{
		obj_col = get_obj_color(&hit);
		return (compute_shadow_ray(obj_col, light, &hit, info));
	}
	if (info->scene->skybox)
		return (draw_skybox(info, &ray));
	return ((t_color){0, 0, 0});
}

static t_color	compute_shadow_ray(t_color obj_col, t_light *light, t_hit *primary_hit, t_rt *info)
{
	t_ray	shadow_ray;
	t_hit	shadow_hit;
	t_vec3	light_vec;
	t_color	final;
	float	light_dist;
	bool	in_shadow;

	if (primary_hit->obj->mirror)
		return ((compute_mirror(obj_col, light, primary_hit, info)));
	//init_shadow_ray(&shadow_ray, primary_hit, light);
	light_vec = v_sub(light->point, primary_hit->hit_point);
	shadow_ray.origin = primary_hit->hit_point; //v_add(p_hit->hit_point, v_scale(p_hit->normal, 1e-4));
	shadow_ray.direction = v_normalize(light_vec);

	light_dist = v_len(light_vec);
	in_shadow = find_hit(&shadow_ray, info, &shadow_hit, true);
	in_shadow = in_shadow && shadow_hit.t > 1e-3 && shadow_hit.t < light_dist;
	ft_bzero(&final, sizeof(t_color));
	if (!in_shadow)
	{
		compute_diffuse(&final, primary_hit, &shadow_ray, light, &obj_col);
		compute_specular(&final, primary_hit, &shadow_ray, light, info->scene->cam);
	}
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
		shadow_color = compute_shadow_ray(obj_color, &info->scene->lights[i], p_hit, info);
		total_color = color_add(total_color, shadow_color);
	}
	total_color = color_clamp(color_add(total_color, amb_color));
	return (color_clamp(total_color));
}
