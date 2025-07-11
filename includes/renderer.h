/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:51:00 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/11 19:28:46 by haaghaja         ###   ########.fr       */
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
	t_vec3	origin;
	t_vec3	direction;
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
	t_vec3	hit_point;
	t_vec3	normal;
	t_obj		*obj;
}	t_hit;

/* render.c */
void	render_scene(t_rt *info);

/* draw.c */
void	img_put_pixel_safe(t_rt *info, int x, int y, int color);

/* skybox.c */
int		draw_skybox(t_rt *info, t_ray *ray);

/* ray.c */
void	init_ray(t_ray *ray, t_rt *info, int x, int y);

/* hit.c */
bool	find_hit(t_ray *ray, t_rt *info, t_hit *hit, bool shadow);

/* plane.c */
float	intersect_plane(t_ray *ray, t_obj *plane);

/* sphere.c */
float 	intersect_sphere(t_ray *ray, t_obj *sphere);

/* cylinder.c */
float	intersect_cylinder(t_ray *ray, t_obj *cylinder);

t_vec3	get_cylinder_normal(t_obj *obj, t_vec3 hit_point);

/* cone.c */
float	intersect_cone(t_ray *ray, t_obj *cone);

t_vec3	get_cone_normal(t_obj *obj, t_vec3 hit_point);

#endif
