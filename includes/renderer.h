/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:51:00 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/08 19:26:50 by azolotar         ###   ########.fr       */
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

void	render_scene(t_rt *info);

void	img_put_pixel_safe(t_rt *info, int x, int y, int color);

#endif
