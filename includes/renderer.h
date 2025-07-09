/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:51:00 by azolotar          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/09 20:36:18 by haaghaja         ###   ########.fr       */
=======
/*   Updated: 2025/07/09 19:46:51 by azolotar         ###   ########.fr       */
>>>>>>> d59fd3a94a1e8cd8addd2d3bf13026c31f532797
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

# include "minirt.h"
# include <stdbool.h>

/*
 * origin is position of cam
 */
typedef struct	s_ray
{
	t_point3	origin;
	t_point3	direction;
}	t_ray;

/*
 * t - distance from ray origin to hit point
 * hit_point - coords of hit
 * normal - ???
 * hit_object - hit object id
 * hit - hit or not
 */
typedef struct	s_hit
{
	float		t;
	t_point3	hit_point;
	t_point3	normal;
	t_obj		*obj;
}	t_hit;

/* render.c */
void	render_scene(t_rt *info);

/* draw.c */
void	img_put_pixel_safe(t_rt *info, int x, int y, int color);

/* ray.c */
void	init_ray(t_ray *ray, t_rt *info, int x, int y);

/* hit.c */
bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit);

int		get_hit_color(t_hit *hit, t_scene *scene);

/* plane.c */
float	intersect_plane(t_ray *ray, t_obj *plane);

/* sphere.c */
<<<<<<< HEAD
float 	intersect_sphere(t_ray *ray, t_obj *sphere);

/* cylinder.c */
float intersect_cylinder(t_ray *ray, t_obj *cylinder);
=======
float	intersect_sphere(t_ray *ray, t_obj *sphere);
>>>>>>> d59fd3a94a1e8cd8addd2d3bf13026c31f532797

#endif
