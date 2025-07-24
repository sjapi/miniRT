/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:28:35 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/24 15:28:06 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "minirt.h"
#include "bounding.h"
#include "utils.h"

void	v_to_max(t_vec3 *v0, t_vec3 *v1)
{
	if (v0->x < v1->x)
		v0->x = v1->x;
	if (v0->y < v1->y)
		v0->y = v1->y;
	if (v0->z < v1->z)
		v0->z = v1->z;
}

void	v_to_min(t_vec3 *v0, t_vec3 *v1)
{
	if (v0->x > v1->x)
		v0->x = v1->x;
	if (v0->y > v1->y)
		v0->y = v1->y;
	if (v0->z > v1->z)
		v0->z = v1->z;
}

static void	set_max_min(t_bvh_node *node, t_obj *obj, int start, int end)
{
	int	i;

	i = start - end;
	while (++i < end)
	{
		v_to_min(&node->aabb_min, &obj[i].aabb_min);
		v_to_max(&node->aabb_max, &obj[i].aabb_max);
	}
}

t_bvh_node	*build_bvh(t_obj *objects, int start, int end)
{
	t_bvh_node	*node;
	t_vec3		extent;
	int			axis;

	node = ft_calloc(sizeof(t_bvh_node), 1);
	if (!node)
		return (NULL);
	v_set(&node->aabb_min, INFINITY);
	v_set(&node->aabb_max, -INFINITY);
	set_max_min(node, objects, start, end);
	if ((end - start) == 1)
	{
		node->object = &objects[start];
		return (node);
	}
	extent = v_sub(node->aabb_max, node->aabb_min);
	axis = 0;
	if (extent.y > extent.x)
		axis = 1;
	if (extent.z > extent.y)
		axis = 2;
	//sort_by_axis(&objects[start], (end - start), axis);
	node->left = build_bvh(objects, start, (end + start) / 2);
	node->right = build_bvh(objects, (end + start) / 2, end);
	return (node);
}
