/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder_helper.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 17:59:15 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 15:46:27 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

float	intersect_base(t_ray *ray, t_obj *cyl, int base);

float	intersect_surface(t_ray *ray, t_obj *cyl);

void	intersect_cyl_all(float *t, t_obj *obj, t_ray *ray, char *side)
{
	float	surface;
	float	top;
	float	bottom;

	surface = intersect_surface(ray, obj);
	top = intersect_base(ray, obj, HIT_TOP);
	bottom = intersect_base(ray, obj, HIT_BOTTOM);
	*t = -1.0;
	if (surface > 0)
	{
		*t = surface;
		*side = HIT_SURFACE;
	}
	if (bottom > 0 && (*t < 0 || bottom < *t))
	{
		*t = bottom;
		*side = HIT_BOTTOM;
	}
	if (top > 0 && (*t < 0 || top < *t))
	{
		*t = top;
		*side = HIT_TOP;
	}
}
