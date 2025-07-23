/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:52:48 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/23 18:38:45 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"

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
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

void	v_set(t_vec3 *v, float value)
{
	v->x = value;
	v->y = value;
	v->z = value;
}
