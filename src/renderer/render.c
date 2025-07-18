/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */ /*                                                +#+#+#+#+#+   +#+           */ /*   Created: 2025/07/09 12:45:57 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/15 19:24:03 by 032zolotarev     ###   ########.fr       */
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

// ============== TESTING =============
long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000));
}
// ====================================

static	void	compute_specular(t_color *final, t_hit *primary_hit, t_ray *shadow_ray, t_light	*light, t_cam *cam) 
{
	float	specular;
	t_color light_col;

	light_col = int_to_color(light->color);
	t_vec3 R = v_sub(
		v_scale(primary_hit->normal, 2 * v_dot(primary_hit->normal, shadow_ray->direction)),
		shadow_ray->direction
	);
	R = v_normalize(R);
	t_vec3 V = v_normalize(v_sub(cam->viewpoint, primary_hit->hit_point));
	specular = powf(clampf(v_dot(R, V), 0, 1), 50);
	t_color specular_col = {
		light_col.r * specular * light->ratio,
		light_col.g * specular * light->ratio,
		light_col.b * specular * light->ratio
	};
	final->r += specular_col.r;
	final->g += specular_col.g;
	final->b += specular_col.b;
}

static	void	compute_diffuse(t_color *final, t_hit *primary_hit, t_ray *shadow_ray, t_light	*light, t_color *obj_col) 
{
	t_color light_col;
	float	diffuse;

	light_col = int_to_color(light->color);
	diffuse = v_dot(primary_hit->normal, shadow_ray->direction);
	diffuse = clampf(diffuse, 0, 1);
	// Diffuse * object * light
	t_color diffuse_col = {
		obj_col->r * light_col.r / 255.0f * diffuse * light->ratio,
		obj_col->g * light_col.g / 255.0f * diffuse * light->ratio,
		obj_col->b * light_col.b / 255.0f * diffuse * light->ratio
	};
	final->r += diffuse_col.r;
	final->g += diffuse_col.g;
	final->b += diffuse_col.b;
}


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

static t_color	decrease_color(t_color color, int amount)
{
    color.r = (color.r > amount) ? color.r - amount : 0;
    color.g = (color.g > amount) ? color.g - amount : 0;
    color.b = (color.b > amount) ? color.b - amount : 0;
	return color;
}

static t_color	compute_shadow_ray(t_light *light, t_hit *primary_hit, t_rt *info);

static t_color	compute_mirror(t_light *light, t_hit *primary_hit, t_rt *info)
{
	t_ray	mirror_ray;
	t_hit	mirror_hit;

	mirror_ray.origin = v_add(primary_hit->hit_point, v_scale(primary_hit->normal, 1e-4));
	mirror_ray.direction = primary_hit->normal;
	if (find_hit(&mirror_ray, info, &mirror_hit, false))
		return (compute_shadow_ray(light, &mirror_hit, info));
		//return (mirror_hit.obj->color);
	return (int_to_color(draw_skybox(info, &mirror_ray)));

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

/*
 * p_hit - primary_hit
 */
static int	compute_color(t_hit *p_hit, t_rt *info)
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
	return (color_to_int(color_clamp(total_color)));
}

void	init_rays_msaa(t_ray rays[4], t_rt *info, int x, int y);

void	render_scene(t_rt *info)
{
	int		y;
	int		x;
	t_ray	rays[4];
	t_hit	hit;
	int		color;
	int		step;
	long stop, start;

	init_optimization(info);
	start = current_time();
	step = (info->mode != RENDER_MODE) + 1;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			t_color	result;
			ft_bzero(&result, sizeof(t_color));
			init_rays_msaa(rays, info, x, y);
			for (int i = 0; i < 4; i++)
			{
				t_ray ray = rays[i];
				ray.origin = info->scene->cam->viewpoint;
				ft_bzero(&hit, sizeof(t_hit));
				if (find_hit(&ray, info, &hit, false))
				{
					color = compute_color(&hit, info);
					img_put_pixel_safe(info, x, y, color);
				}
				else
				{
					if (info->scene->skybox)
						color = draw_skybox(info, &ray);
					else
						color = info->scene->amb->color * info->scene->amb->ratio;
				}
				result = color_add(result, int_to_color(color));
			}
			result.r /= 4;
			result.g /= 4;
			result.b /= 4;
			result = color_clamp(result);
			color = color_to_int(result);
			img_put_pixel_safe(info, x, y, color);
			if (step != 1)
			{
				img_put_pixel_safe(info, x + 1, y, color);
				img_put_pixel_safe(info, x, y + 1, color);
				img_put_pixel_safe(info, x + 1, y + 1, color);
			}
			x += step;
		}
		y += step;
	}
	draw_xyz_axis(info);
	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
	stop = current_time();
	printf("Time %ld\n", stop - start);
	draw_info(info);
}
