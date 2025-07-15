/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:16:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/15 16:05:15 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include <stdbool.h>

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

typedef struct s_texture
{
	char	*file_name;
	void	*mlx;
	char	*data;
	int		width;
	int		height;
	int		bpp;
	int		line_length;
	int		endian;
}	t_texture;

/*
 * type 1 = square
 * type 2 = sphere
 * type 3 = plane
 *
 * p.s only sphere does not have norm vector (pl, cy, co have)
 * added bool and visible, need to implement in future
 */
typedef struct s_obj
{
	char		type;
	t_vec3	center;
	int			color;
	t_vec3	norm_vector; // for plane??
	float		*attrs;
	float		bounding_r;
	bool		selected;
	bool		checkerboard;
	bool		visible;
	t_texture	*texture;
}	t_obj;

/*
 * yaw and pitch - self made for camera rotation
 */
typedef struct s_cam
{
	t_vec3	viewpoint;
	t_vec3	orient_v;
	int		fov;

	float	yaw;
	float	pitch;

	t_vec3	right;
	t_vec3	up;
}	t_cam;

/*
 * IMO: parser should convert it to int val
 * color = 0xFF00FF (255,0,255)
 * wanted to delete this struct, cause it useless in miniRT
 * but decided to leave it, cause it may be helpfull if we'll decide to change ration during rendering
 * (idk why)
 * and to calculate final ambient light color
 */
typedef struct s_amb_light
{
	float	ratio;
	int		color;
}	t_amb_light;

typedef struct s_light
{
	t_vec3	point;
	float		ratio;
	int			color;
}	t_light;

/*
 * need to figure out can be more than one light?
 * yep, can be, in bonus part (which we will do)
 */
typedef struct s_scene
{
	t_obj		*objs;
	int			objs_count;

	t_amb_light	*amb;
	t_cam		*cam;
	t_light		*lights;
	t_texture	*skybox;
	int			lights_count;
	t_obj		*selected;
}	t_scene;

typedef struct	s_optim
{
	float	*viewport_x;
	float	*viewport_y;
}	t_optim;

typedef struct s_rt
{
	void	*mlx;
	void	*win;
	void	*img;

	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;

	float	win_aspect_ratio;
	t_optim	*optim;
	t_scene	*scene;
}	t_rt;

#endif
