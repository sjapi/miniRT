/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:56:54 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/20 18:54:49 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "minirt.h"

t_vec3	v_min(t_vec3 *v0, t_vec3 *v1)
{
	t_vec3	min;

	min.x = fminf(v0->x, v1->x);
	min.y = fminf(v0->y, v1->y);
	min.z = fminf(v0->z, v1->z);
	return (min);
}

t_vec3	v_max(t_vec3 *v0, t_vec3 *v1)
{
	t_vec3	max;

	max.x = fminf(v0->x, v1->x);
	max.y = fminf(v0->y, v1->y);
	max.z = fminf(v0->z, v1->z);
	return (max);
}

void	find_selected(t_scene *scene)
{
	int	i;

	i = 0;
	while (i++ < scene->objs_count)
	{
		if (scene->objs[i].selected)
			scene->selected = &scene->objs[i];
	}
}
