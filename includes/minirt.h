/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:16:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/05 19:31:11 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include "parser.h"
#include "utils.h"


typedef struct	s_point3
{
	float	x;
	float	y;
	float	z;
}	t_point3;

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
	t_point3	center;
	int			color;
	t_point3	norm_vector;
	float		*attrs;
}	t_obj;

typedef struct	s_cam
{
	t_point3	viewpoint;
	t_point3	orient_v;
	char		field_of_view;
}	t_cam;

/*
 * IMO: parser should convert it to int val
 * color = 0xFF00FF (255,0,255)
 */
typedef struct	s_amb_light
{
	float	ratio;
	int		color;
}	t_amb_light;

typedef struct	t_light
{
	t_point3	point;
	float		bright_ratio;
	int		color;
}	t_light;

/*
 * need to figure out can be more than one light?
 */
typedef struct	s_scene
{
	t_obj		*objs;
	int			objs_count;

	t_amb_light	*light;
	t_cam		*cam;
	t_light		*lights;
}	t_scene;

typedef struct	s_rt
{
	void	*mlx;
	void	*win;
	void	*img;

	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_rt;

#endif
