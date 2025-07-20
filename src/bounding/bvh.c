/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:28:35 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/20 18:51:39 by haaghaja         ###   ########.fr       */
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

t_bvh_node *build_bvh(t_obj *objects, int start, int end)
{
	printf("Called\n");
    t_bvh_node *node = malloc(sizeof(t_bvh_node));
    if (!node)
        return NULL;

    // Compute combined AABB
    node->aabb_min = (t_vec3){ INFINITY, INFINITY, INFINITY };
    node->aabb_max = (t_vec3){ -INFINITY, -INFINITY, -INFINITY };
    for (int i = start; i < end; i++)
    {
        v_to_min(&node->aabb_min, &objects[i].aabb_min);
        v_to_max(&node->aabb_max, &objects[i].aabb_max);
    }
    int count = end - start;
    if (count == 1)
    {
        // Leaf
        node->left = node->right = NULL;
        node->object = &objects[start];
        return node;
    }

    // Choose axis with largest extent
    t_vec3 extent = v_sub(node->aabb_max, node->aabb_min);
    int axis = 0;
    if (extent.y > extent.x) axis = 1;
    if (extent.z > extent.y) axis = 2;

    // Sort objects by AABB center along chosen axis
    sort_by_axis(&objects[start], count, axis);

    // Split in half
    int mid = start + count / 2;
    node->left = build_bvh(objects, start, mid);
    node->right = build_bvh(objects, mid, end);
    node->object = NULL;
    return node;
}

