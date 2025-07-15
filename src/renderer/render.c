/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:45:57 by 032zolotarev      #+#    #+#             */
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

	u = 0.5 + atan2(p.z, p.x) / (2 * M_PI);
	v = 0.5 - asin(p.y / radius) / M_PI;

	sx = clamp((int)(u * hit->obj->texture->width), 0, hit->obj->texture->width - 1);
	sy = clamp((int)(v * hit->obj->texture->height), 0, hit->obj->texture->height - 1);

	pixel = hit->obj->texture->data + sy * hit->obj->texture->line_length + sx * (hit->obj->texture->bpp / 8);
	int color_int = *(int *)pixel;
	return int_to_color(color_int);
}

int decrease_color(int color, int amount)
{
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;

    r = (r > amount) ? r - amount : 0;
    g = (g > amount) ? g - amount : 0;
    b = (b > amount) ? b - amount : 0;

    return (r << 16) | (g << 8) | b;
}

static	int	compute_mirror(t_hit *primary_hit, t_rt *info)
{
	t_ray	mirror_ray;
	t_hit	mirror_hit;

	mirror_ray.origin = v_add(primary_hit->hit_point, v_scale(primary_hit->normal, 1e-4));
	mirror_ray.direction = primary_hit->normal;
	if (find_hit(&mirror_ray, info, &mirror_hit, false))
		return (mirror_hit.obj->color);
	return (draw_skybox(info, &mirror_ray));

}


static int	compute_lighting(t_hit *primary_hit, t_rt *info)
{
	t_ray	shadow_ray;
	t_hit	shadow_hit;
	t_color	final = {0, 0, 0};
	float	light_dist;
	bool	in_shadow;

	if (primary_hit->obj->mirror)
		return (decrease_color(compute_mirror(primary_hit, info), 30));
	t_color	obj_col = compute_object_color(primary_hit);
	if (primary_hit->reverse)
	{
		obj_col.r = 255.0 - obj_col.r;
		obj_col.g = 255.0 - obj_col.g;
		obj_col.b = 255.0 - obj_col.b;
	}
	t_color	amb_col = int_to_color(get_amb_color(info->scene->amb));
	t_color light_col = int_to_color(info->scene->lights->color);
	shadow_ray.origin = v_add(primary_hit->hit_point, v_scale(primary_hit->normal, 1e-4));
	shadow_ray.direction = v_sub(info->scene->lights->point, primary_hit->hit_point);
	shadow_ray.direction = v_normalize(shadow_ray.direction);
	light_dist = v_len(v_sub(info->scene->lights->point, primary_hit->hit_point));
	in_shadow = false;
	if (find_hit(&shadow_ray, info, &shadow_hit, true))
	{
		if (shadow_hit.t > 1e-3 && shadow_hit.t < light_dist)
			in_shadow = true;
	}
	// Compute diffuse
	if (!in_shadow)
	{
		compute_diffuse(&final, primary_hit, &shadow_ray, &info->scene->lights[0], &obj_col);
		compute_specular(&final, primary_hit, &shadow_ray, &info->scene->lights[0], info->scene->cam);
	}
	// Ambient * object
	t_color ambient_col = {
		obj_col.r * amb_col.r / 255.0f * info->scene->amb->ratio,
		obj_col.g * amb_col.g / 255.0f * info->scene->amb->ratio,
		obj_col.b * amb_col.b / 255.0f * info->scene->amb->ratio
	};
	// Final color
	final.r = clamp(final.r + ambient_col.r, 0, 255);
	final.g = clamp(final.g + ambient_col.g, 0, 255);
	final.b = clamp(final.b + ambient_col.b, 0, 255);
	return color_to_int(final);
}

void	render_scene(t_rt *info)
{
	int		y;
	int		x;
	t_ray	ray;
	t_hit	hit;
	int		color;
	long stop, start;

	init_optimization(info);
	start = current_time();
	ray.origin = info->scene->cam->viewpoint;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		/*
		if (y % 2 == 0)
		{
			y++;
			continue ;
		}
		*/
		while (x < WIN_WIDTH)
		{
			/*
			if (x % 2 == 0)
			{
				x++;
				continue ;
			}
			*/
			init_ray(&ray, info, x, y);
			hit.contour = false;
			hit.reverse = false;
			if (find_hit(&ray, info, &hit, false))
			{
				color = compute_lighting(&hit, info);
				img_put_pixel_safe(info, x, y, color);
			}
			else
			{
				if (info->scene->skybox)
					color = draw_skybox(info, &ray);
				else
					color = info->scene->amb->color * info->scene->amb->ratio;
				img_put_pixel_safe(info, x, y, color);
			}
			x++;
		}
		y++;
	}
	draw_xyz_axis(info);
	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
	stop = current_time();
	printf("Time %ld\n", stop - start);
	draw_info(info);
}
