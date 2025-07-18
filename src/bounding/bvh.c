#include <stdlib.h>
#include "minirt.h"
#include "bounding.h"

t_bvh_node *build_bvh(t_obj **objects, int start, int end)
{
    t_bvh_node *node = malloc(sizeof(t_bvh_node));
    if (!node)
        return NULL;

    // Compute combined AABB
    node->aabb_min = (t_vec3){ INFINITY, INFINITY, INFINITY };
    node->aabb_max = (t_vec3){ -INFINITY, -INFINITY, -INFINITY };
    for (int i = start; i < end; ++i)
    {
        node->aabb_min = v_min(node->aabb_min, objects[i]->aabb_min);
        node->aabb_max = v_max(node->aabb_max, objects[i]->aabb_max);
    }

    int count = end - start;
    if (count == 1)
    {
        // Leaf
        node->left = node->right = NULL;
        node->object = objects[start];
        return node;
    }

    // Choose axis with largest extent
    t_vec3 extent = v_sub(node->aabb_max, node->aabb_min);
    int axis = 0;
    if (extent.y > extent.x) axis = 1;
    if (extent.z > extent[axis]) axis = 2;

    // Sort objects by AABB center along chosen axis
    sort_by_axis(objects + start, count, axis);

    // Split in half
    int mid = start + count / 2;
    node->left = build_bvh(objects, start, mid);
    node->right = build_bvh(objects, mid, end);
    node->object = NULL;
    return node;
}

