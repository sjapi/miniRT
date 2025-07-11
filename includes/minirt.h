/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:16:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/09 20:59:48 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

typedef struct s_point3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

/*
 * type 1 = square
 * type 2 = sphere
 * type 3 = plane
 *
 * p.s only sphere does not have norm vector (pl, cy, co have)
 */
typedef struct s_obj
{
	char		type;
	t_vec3	center;
	int			color;
	t_vec3	norm_vector; // for plane??
	float		*attrs;
}	t_obj;

/*
 * yaw and pitch - self made for camera rotation
 */
typedef struct s_cam
{
	t_vec3	viewpoint;
	t_vec3	orient_v;
	int			fov;

	float		yaw;
	float		pitch;
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

typedef struct t_light
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
	int			lights_count;


	void    *skybox;
	char    *skybox_data;
	int     skybox_width;
	int     skybox_height;
	int     skybox_bpp;
	int     skybox_line_length;
	int     skybox_endian;

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
