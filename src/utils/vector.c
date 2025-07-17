/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 09:45:42 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/17 19:50:54 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdbool.h>
#include <math.h>

static bool	in_range(float val, float min, float max)
{
	return (val >= min && val <= max);
}

float	v_len(t_vec3 v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

bool	v_normalized(t_vec3 p)
{
	return (fabsf(v_len(p) - 1.0f) < 1e-6f);
}

t_vec3	v_add(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec3	v_scale(t_vec3 v, float s)
{
	return ((t_vec3){v.x * s, v.y * s, v.z * s});
}

/* cross product */
t_vec3	v_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	});
}

t_vec3	v_normalize(t_vec3 v)
{
	float	len;

	len = v_len(v);
	return ((t_vec3){v.x / len, v.y / len, v.z / len});
}

/* dot product */
float	v_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

/* substraction */
t_vec3	v_sub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	});
}
