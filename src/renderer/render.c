/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:45:57 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/13 22:05:12 by 032zolotarev     ###   ########.fr       */
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

/* static bool	in_range(float val, float min, float max)
{
	return (val >= min && val <= max);
}

bool	is_hitable(t_vec3 tmp, t_vec3 obj, t_vec3 light)
{
	float	p1[3];
	float	p2[3];
	float	p3[3];
	int	a = 0;

	p1[0] = tmp.x;	
	p1[1] = tmp.y;	
	p1[2] = tmp.z;	

	p2[0] = light.x;	
	p2[1] = light.y;	
	p2[2] = light.z;	
	
	p3[0] = obj.x;	
	p3[1] = obj.y;	
	p3[2] = obj.z;
	
	//printf("%f %f %f\n%f %f %f\n%f %f %f\n", p1[0], p1[1], p1[2], p3[0], p3[1], p3[2], p2[0], p2[1], p2[2]); 
	// For X
	if (p1[0] > p2[0] && in_range(p3[0], p2[0], p1[0]))
			a++;
	else if (p1[0] < p2[0] && in_range(p3[0], p1[0], p2[0]))
			a++;
	// For y
	if (p1[1] > p2[1] && in_range(p3[1], p2[1], p1[1]))
			a++;
	else if (p1[1] < p2[1] && in_range(p3[1], p1[1], p2[1]))
			a++;
	// For z
	if (p1[2] > p2[2] && in_range(p3[2], p2[2], p1[2]))
			a++;
	else if (p1[2] < p2[2] && in_range(p3[2], p1[2], p2[2]))
			a++;
	//printf("%d\n", a);
	return (a == 3);
}

bool	_test_hit(t_ray *ray, t_rt *info, t_hit *hit)
{
	int		i;
	float	t;
	float	closest = 270000;
	t_obj	*obj;
	bool	find = false;

	i = -1;
	while (++i < info->scene->objs_count)
	{
		t = -1;
		obj = &info->scene->objs[i];
		if (!is_hitable(ray->origin, info->scene->lights->point, obj->center))
			continue ;
		if (obj->type == PLANE)
			t = intersect_plane(ray, obj);
		else if (obj->type == SPHERE)
			t = intersect_sphere(ray, obj);
		else if (obj->type == CYLINDER)
			t = intersect_cylinder(ray, obj, NULL);
		else if (obj->type == CONE)
			t = intersect_cone(ray, obj, NULL);
		if (t > 0 && t < closest)
		{
			find = true;
			hit->t = t;
			closest = t;
			hit->hit_point = v_add(ray->origin, v_scale(ray->direction, t));
		}
	}
	return (find);
}
// ====================================
*/

static	void	compute_specular(t_color *final, t_hit *primary_hit, t_ray *shadow_ray, t_light	*light, t_cam *cam) 
{
	float	specular;
	t_color light_col;

	light_col = int_to_color(light->color);
	specular = 0.0f;
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

static int	compute_lighting(t_hit *primary_hit, t_rt *info)
{
	t_ray	shadow_ray;
	t_hit	shadow_hit;
	t_color	final = {0, 0, 0};
	float	light_dist;
	bool	in_shadow;

	if (primary_hit->contour)
			return 0xff0000;
	t_color obj_col = int_to_color(primary_hit->obj->color);
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
	float diffuse = 0.0f;
	if (!in_shadow)
	{
		diffuse = v_dot(primary_hit->normal, shadow_ray.direction);
		diffuse = clampf(diffuse, 0, 1);
		// Specular
		compute_specular(&final, primary_hit, &shadow_ray, &info->scene->lights[0], info->scene->cam);
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

	// Final color
	final.r = clamp(final.r + ambient_col.r + diffuse_col.r, 0, 255);
	final.g = clamp(final.g + ambient_col.g + diffuse_col.g, 0, 255);
	final.b = clamp(final.b + ambient_col.b + diffuse_col.b, 0, 255);
	return color_to_int(final);
}

void	render_scene(t_rt *info)
{
	int		y;
	int		x;
	t_ray	ray;
	int		color;
	long stop, start;

	start = current_time();
	ray.origin = info->scene->cam->viewpoint;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			init_ray(&ray, info, x, y);
			t_hit	hit;
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
	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
	stop = current_time();
	printf("Time %ld\n", stop - start);
}
