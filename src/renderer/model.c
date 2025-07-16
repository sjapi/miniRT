/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:54:34 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/16 21:23:01 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

float intersect_triangle(t_ray *ray, t_vec3 v0, t_vec3 v1, t_vec3 v2, bool *reverse)
{
    (void)reverse; // not used for now

    const float EPSILON = 1e-6;
    t_vec3 edge1 = v_sub(v1, v0);
    t_vec3 edge2 = v_sub(v2, v0);

    t_vec3 h = v_cross(ray->direction, edge2);
    float a = v_dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return -1; // parallel

    float f = 1.0f / a;
    t_vec3 s = v_sub(ray->origin, v0);
    float u = f * v_dot(s, h);
    if (u < 0.0f || u > 1.0f)
        return -1;

    t_vec3 q = v_cross(s, edge1);
    float v = f * v_dot(ray->direction, q);
    if (v < 0.0f || u + v > 1.0f)
        return -1;

    float t = f * v_dot(edge2, q);
    if (t > EPSILON)
        return t;

    return -1;
}

t_vec3	get_model_normal(t_mesh *mesh, t_vec3 hit_point, t_vec3 ray_dir, int tri_j)
{
    t_vec3 v0 = mesh->points[(int)mesh->triangles[tri_j + 0]];
    t_vec3 v1 = mesh->points[(int)mesh->triangles[tri_j + 1]];
    t_vec3 v2 = mesh->points[(int)mesh->triangles[tri_j + 2]];

    t_vec3 edge1 = v_sub(v1, v0);
    t_vec3 edge2 = v_sub(v2, v0);

    t_vec3 normal = v_cross(edge1, edge2);
    normal = v_normalize(normal);

    // Ensure normal faces against ray direction
    if (v_dot(normal, ray_dir) > 0)
        normal = v_scale(normal, -1);

    return normal;
}

float intersect_model(t_ray *ray, t_obj *obj, t_hit *hit, int *tj, bool *reverse)
{
    float t_min = -1;
    int j = 0;

    while (j < obj->mesh->size)
    {
        t_vec3 v0 = v_add(obj->mesh->points[obj->mesh->triangles[j + 0]], obj->center);
        t_vec3 v1 = v_add(obj->mesh->points[obj->mesh->triangles[j + 1]], obj->center);
        t_vec3 v2 = v_add(obj->mesh->points[obj->mesh->triangles[j + 2]], obj->center);

        float t = intersect_triangle(ray, v0, v1, v2, reverse);
        if (t > 0 && (t_min < 0 || t < t_min))
            t_min = t;
			*tj = j;
        j += 3;
    }
    return t_min;
}

