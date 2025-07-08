/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:38:47 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/08 19:34:09 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>

static t_point3	get_ray_dir(float nx, float ny, t_cam *cam)
{
	t_point3	forward;
	t_point3	world_up;
	t_point3	right;
	t_point3	up;
	t_point3	dir;

	forward = cam->orient_v;
	if (fabsf(forward.x) < 1e-6 && fabsf(forward.z) < 1e-6)
		world_up = (t_point3){0, 0, 1};
	else
		world_up = (t_point3){0, 1, 0};
	right = v_normalize(v_cross(world_up, forward));
	up = v_cross(forward, right);
	dir = v_add(
		v_add(forward, v_scale(right, nx)),
		v_scale(up, ny)
	);
	return (v_normalize(dir));
}

/*
 * at first nx, ny - normalized coordinates in range 0...1
 * and after they are converted to range -1...1 and scaled to actual size of viewport
 */
static void	init_ray(t_ray *ray, t_rt *info, int x, int y)
{
	float		nx;
	float		ny;
	float		tan_fov;

	nx = (float)(x + 0.5) / (float)WIN_WIDTH;
	ny = (float)(y + 0.5) / (float)WIN_HEIGHT;
	tan_fov = tanf(info->scene->cam->fov / 2.0);
	nx = (2.0 * nx - 1.0) * info->win_aspect_ratio * tan_fov;
	ny = (1.0 - 2.0 * ny) * tan_fov;
	ray->origin = info->scene->cam->view_point;
	ray->direction = get_ray_dir(nx, ny, info->scene->cam);
}

static int	intersect_plane(t_ray *ray, t_obj *plane)
{
	float	denom;
	float	t;

	denom = v_dot(ray->direction, plane->norm_vector);
	if (fabsf(denom) < 1e-6)
		return (-1);
	t = v_dot(v_sub(plane->center, ray->origin), plane->norm_vector) / denom;
	if (t < 0)
		return (-1);
	return (t);
}

static int intersect_sphere(t_ray *ray, t_obj *sphere)
{
    float radius = sphere->attrs[0] * 0.5f;
    t_point3 oc = v_sub(ray->origin, sphere->center);

    float a = v_dot(ray->direction, ray->direction);
    float b = 2.0f * v_dot(oc, ray->direction);
    float c = v_dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (-1);

    float sqrt_disc = sqrtf(discriminant);
    float t0 = (-b - sqrt_disc) / (2 * a);
    float t1 = (-b + sqrt_disc) / (2 * a);

    if (t0 > 1e-6)
        return (t0);
    if (t1 > 1e-6)
        return (t1);
    return (-1);
}


static bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit)
{
	int		i;
	float	t;
	t_obj	*obj;
	bool	find = false;

	i = -1;
	while (++i < info->scene->objs_count)
	{
		t = -1;
		obj = &info->scene->objs[i];
		if (obj->type == PLANE)
			t = intersect_plane(ray, obj);
		else if (obj->type == SPHERE)
		{
			t = intersect_sphere(ray, obj);
		}
		// else work with other objs
		if (t > 0)
		{
			find = true;
			hit->t = t;
			hit->hit_point = v_add(ray->origin, v_scale(ray->direction, t));
			hit->normal = obj->norm_vector;
			hit->obj = obj;
		}
	}
	return (find);
}

int count = 0;

static int	get_hit_color(t_hit *hit, t_scene *scene)
{
    int obj = hit->obj->color;
    int amb = scene->amb->color;
    float ratio = scene->amb->ratio;

    int r = ((obj >> 16) & 0xFF) * ((amb >> 16) & 0xFF) / 255 * ratio;
    int g = ((obj >> 8) & 0xFF) * ((amb >> 8) & 0xFF) / 255 * ratio;
    int b = (obj & 0xFF) * (amb & 0xFF) / 255 * ratio;

    return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

void	render_scene(t_rt *info)
{
	int		y = 0;
	int		x;
	int		amb = get_amb_color(info->scene->amb);
	t_ray	ray;
	t_hit	hit;

	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			init_ray(&ray, info, x, y);
			if (find_hit(&ray, info, &hit))
			{
				count += 1;
				int color = get_hit_color(&hit, info->scene);
				img_put_pixel_safe(info, x, y, color);
			}
			else
			{
				img_put_pixel_safe(info, x, y, amb);
			}
			x++;
		}
		y++;
	}
	printf("%d/%d\n", count, WIN_WIDTH * WIN_HEIGHT);
	mlx_put_image_to_window(info->mlx, info->win, info->img, 0, 0);
}
