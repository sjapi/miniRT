#ifndef BOUNDING_H
# define BOUNDING_H

# include <stdbool.h>
# include "minirt.h"

typedef struct s_bvh_node {
    t_vec3	aabb_min;
    t_vec3	aabb_max;
    struct	s_bvh_node *left;
    struct	s_bvh_node *right;
    t_obj *object;
} t_bvh_node;

/* bvh.c */
t_bvh_node	*build_bvh(t_obj **objects, int start, int end);

#endif
