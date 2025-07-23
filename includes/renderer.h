/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:51:00 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/23 14:03:11 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

# include "minirt.h"
# include "utils.h"
# include <stdbool.h>

/*
 * origin is position of cam
 */
typedef struct s_ray
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
 * reverse - for checkerboard
 */
typedef struct s_hit
{
	float	t;
	t_vec3	hit_point;
	t_vec3	normal;
	int		tri_i;
	t_obj	*obj;
	char	side;
	bool	reverse;
}	t_hit;

/* optimization */
bool	init_optimization(t_rt *info);

/* render.c */
void	render(t_rt *info);

/* draw.c */
void	img_put_pixel_safe(t_rt *info, int x, int y, int color);
void	img_draw_line(t_rt *info, t_vec3 a, t_vec3 b, int color);
void	draw_xyz_axis(t_rt *info);

/* skybox.c */
t_color	draw_skybox(t_rt *info, t_ray *ray);

/* ray.c */
void	init_ray(t_ray *ray, t_rt *info, int x, int y);
void	init_rays_msaa(t_ray rays[4], t_rt *info, int x, int y);

t_vec3	get_ray_dir(float nx, float ny, t_cam *cam);

/* hit.c */
bool	find_hit(t_ray *ray, t_scene *scene, t_hit *hit);
bool	is_in_shadow(t_ray *ray, t_scene *scene, float d);

/* intersection */
float	intersect_plane(t_ray *ray, t_obj *plane, bool *reverse);
float	intersect_sphere(t_ray *ray, t_obj *sphere, bool *reverse);
float	intersect_cylinder(
			t_ray *ray, t_obj *cylinder, char *side, bool *reverse);
float	intersect_cone(
			t_ray *ray, t_obj *cone, char *side, bool *reverse);
float	intersect_model(
			t_ray *ray, t_obj *obj, t_hit *hit, int *tj, bool *reverse);

/* normal */
t_vec3	get_cylinder_normal(
			t_obj *obj, t_vec3 hit_point, t_vec3 ray_dir, char side);
t_vec3	get_cone_normal(
			t_obj *obj, t_vec3 hit_point, t_vec3 ray_dir, char side);
t_vec3	get_model_normal(
			t_obj *obj, t_vec3 hit_point, t_vec3 ray_dir, int tri_j);

/* checkerboard */
bool	sphere_checkerboard(t_vec3 hit_point, t_obj *sphere);
bool	cylinder_checkerboard(t_vec3 hit_point, t_obj *cyl, bool base);
bool	plane_checkerboard(t_vec3 hit_point, t_obj *plane);
bool	cone_checkerboard(t_vec3 hit_point, t_obj *cone, bool base);

/* info */
void	draw_info(t_rt *info);

void	compute_specular(t_color *final, t_hit *phit, t_ray *sray, t_light	*l, t_cam *c);
void	compute_diffuse(t_color *final, t_hit *phit, t_ray *sray, t_light *l, t_color *obj_col);
t_color	compute_color(t_hit *p_hit, t_rt *info);

#endif
