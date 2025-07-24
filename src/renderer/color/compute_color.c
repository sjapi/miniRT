/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:10:39 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 16:17:10 by haaghaja         ###   ########.fr       */
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

t_color	get_obj_color(t_hit *hit)
{
	t_color	color;

	if (hit->obj->texture == NULL)
		color = int_to_color(hit->obj->color);
	else
		color = get_texture_color(hit);
	if (hit->reverse && hit->obj->type != MODEL)
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

t_color	compute_shadow_ray(t_color obj_col, t_light *light,
					t_hit *p_hit, t_rt *info)
{
	t_ray	shadow_ray;
	t_vec3	light_vec;
	t_color	final;

	if (p_hit->obj->mirror && p_hit->obj->type == SPHERE)
		return ((compute_mirror(obj_col, light, p_hit, info)));
	ft_bzero(&final, sizeof(t_color));
	light_vec = v_sub(light->point, p_hit->hit_point);
	shadow_ray.origin = p_hit->hit_point;
	shadow_ray.direction = v_normalize(light_vec);
	if (is_in_shadow(&shadow_ray, info->scene, v_len(light_vec)))
		return (final);
	final = color_add(final, compute_diffuse(p_hit, &shadow_ray,
				light, &obj_col));
	final = color_add(final, compute_specular(p_hit, &shadow_ray,
				light, info->scene->cam));
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
