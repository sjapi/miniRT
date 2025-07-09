/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:54:16 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/09 19:46:22 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include <math.h>

float	intersect_plane(t_ray *ray, t_obj *plane)
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
