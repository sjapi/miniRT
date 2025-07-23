/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mirror.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:06:01 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/23 20:23:36 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "renderer.h"
#include "minirt.h"
#include "utils.h"

t_color	compute_shadow_ray(t_color obj_col, t_light *light, t_hit *p_hit, t_rt *info);

t_color	get_obj_color(t_hit *hit);

t_color	compute_mirror(t_color obj_col, t_light *light, t_hit *p_hit, t_rt *info)
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
