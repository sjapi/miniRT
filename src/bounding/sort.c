/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:37:57 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/23 20:01:58 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static float	get_center(const t_obj *obj, int axis)
{
	if (axis == 0)
		return ((obj->aabb_min.x + obj->aabb_max.x) * 0.5f);
	if (axis == 1)
		return ((obj->aabb_min.y + obj->aabb_max.y) * 0.5f);
	return ((obj->aabb_min.z + obj->aabb_max.z) * 0.5f);
}

static void	swap_obj(t_obj *a, t_obj *b)
{
	t_obj	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	partition(t_obj *objects, int low, int high, int axis)
{
	float	pivot;
	int		i;
	int		j;

	pivot = get_center(&objects[high], axis);
	i = low - 1;
	j = low;
	while (++j < high)
	{
		if (get_center(&objects[j], axis) < pivot)
		{
			++i;
			swap_obj(&objects[i], &objects[j]);
		}
	}
	swap_obj(&objects[i + 1], &objects[high]);
	return (i + 1);
}

static void	quicksort_objs(t_obj *objects, int low, int high, int axis)
{
	int	pi;

	if (low < high)
	{
		pi = partition(objects, low, high, axis);
		quicksort_objs(objects, low, pi - 1, axis);
		quicksort_objs(objects, pi + 1, high, axis);
	}
}

void	sort_by_axis(t_obj *objects, int count, int axis)
{
	if (count > 1)
		quicksort_objs(objects, 0, count - 1, axis);
}
