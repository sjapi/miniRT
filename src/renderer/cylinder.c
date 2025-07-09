/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:35:13 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/09 20:36:40 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include "defines.h"
#include <math.h>

float intersect_cylinder(t_ray *ray, t_obj *cylinder)
{
    float radius = cylinder->attrs[CYLINDER_D_I] * 0.5f;
    float height = cylinder->attrs[CYLINDER_H_I];
    t_point3 oc = v_sub(ray->origin, cylinder->center);
    t_point3 axis = cylinder->norm_vector;

    // Project ray direction and oc onto plane orthogonal to cylinder axis
    float d_dot_a = v_dot(ray->direction, axis);
    float oc_dot_a = v_dot(oc, axis);

    t_point3 d_perp = v_sub(ray->direction, v_scale(axis, d_dot_a));
    t_point3 oc_perp = v_sub(oc, v_scale(axis, oc_dot_a));

    float a = v_dot(d_perp, d_perp);
    float b = 2.0f * v_dot(d_perp, oc_perp);
    float c = v_dot(oc_perp, oc_perp) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (-1);

    float sqrt_disc = sqrtf(discriminant);
    float t0 = (-b - sqrt_disc) / (2 * a);
    float t1 = (-b + sqrt_disc) / (2 * a);

    // Check each intersection point to see if it lies within the cylinder height
    float t;
    t = t0;
    if (t > 1e-6)
    {
        t_point3 p = v_add(ray->origin, v_scale(ray->direction, t));
        float h = v_dot(v_sub(p, cylinder->center), axis);
        if (h >= 0 && h <= height)
            return t;
    }
    t = t1;
    if (t > 1e-6)
    {
        t_point3 p = v_add(ray->origin, v_scale(ray->direction, t));
        float h = v_dot(v_sub(p, cylinder->center), axis);
        if (h >= 0 && h <= height)
            return t;
    }

    return (-1);
}

