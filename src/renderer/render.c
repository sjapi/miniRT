/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:45:57 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/11 15:51:52 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>

static int	compute_lighting(t_hit *primary_hit, t_rt *info)
{
	t_ray	shadow_ray;
	t_hit	shadow_hit;
	float	light_dist;
	bool	in_shadow;

	t_color	amb_col = int_to_color(get_amb_color(info->scene->amb));
	t_color obj_col = int_to_color(primary_hit->obj->color);
	t_color light_col = int_to_color(info->scene->lights->color);

	shadow_ray.origin = v_add(primary_hit->hit_point, v_scale(primary_hit->normal, 1e-4));
	shadow_ray.direction = v_sub(info->scene->lights->point, primary_hit->hit_point);
	shadow_ray.direction = v_normalize(shadow_ray.direction);
	light_dist = v_len(v_sub(info->scene->lights->point, primary_hit->hit_point));
	in_shadow = false;
	if (find_hit(&shadow_ray, info, &shadow_hit))
	{
		if (shadow_hit.t > 1e-3 && shadow_hit.t < light_dist)
			in_shadow = true;
	}
	// Compute diffuse
	float diffuse = 0.0f;
	float specular = 0.0f;
	if (!in_shadow)
	{
		diffuse = v_dot(primary_hit->normal, shadow_ray.direction);
		diffuse = clampf(diffuse, 0, 1);
		t_vec3 R = v_sub(
			v_scale(primary_hit->normal, 2 * v_dot(primary_hit->normal, shadow_ray.direction)),
			shadow_ray.direction
		);
		R = v_normalize(R);
		t_vec3 V = v_normalize(v_sub(info->scene->cam->viewpoint, primary_hit->hit_point));
		specular = powf(clampf(v_dot(R, V), 0, 1), 50);
	}
	// Ambient * object
	t_color ambient_col = {
		obj_col.r * amb_col.r / 255.0f * info->scene->amb->ratio,
		obj_col.g * amb_col.g / 255.0f * info->scene->amb->ratio,
		obj_col.b * amb_col.b / 255.0f * info->scene->amb->ratio
	};
	// Diffuse * object * light
	t_color diffuse_col = {
		obj_col.r * light_col.r / 255.0f * diffuse * info->scene->lights->ratio,
		obj_col.g * light_col.g / 255.0f * diffuse * info->scene->lights->ratio,
		obj_col.b * light_col.b / 255.0f * diffuse * info->scene->lights->ratio
	};

	t_color specular_col = {
		light_col.r * specular * info->scene->lights->ratio,
		light_col.g * specular * info->scene->lights->ratio,
		light_col.b * specular * info->scene->lights->ratio
	};

	// Final color
	t_color final = {
		clamp(ambient_col.r + diffuse_col.r + specular_col.r, 0, 255),
		clamp(ambient_col.g + diffuse_col.g + specular_col.g, 0, 255),
		clamp(ambient_col.b + diffuse_col.b + specular_col.b, 0, 255)
	};
	return color_to_int(final);
}

void	render_scene(t_rt *info)
{
	int		y;
	int		x;
	t_ray	ray;
	t_hit	hit;
	int		color;

	ray.origin = info->scene->cam->viewpoint;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			init_ray(&ray, info, x, y);
			if (find_hit(&ray, info, &hit))
			{
				
				color = compute_lighting(&hit, info);
				img_put_pixel_safe(info, x, y, color);
			}
			else
			{
				color = draw_skybox(info, &ray);
				img_put_pixel_safe(info, x, y, color);
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
}
