/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:39:18 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/20 20:00:44 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"
#include <math.h>

void	compute_specular(t_color *final,
			t_hit *phit, t_ray *sray, t_light	*l, t_cam *c)
{
	float	specular;
	t_color	light_col;
	t_vec3	r;
	t_vec3	v;
	t_color	spec;

	light_col = int_to_color(l->color);
	r = v_sub(
			v_scale(phit->normal, 2 * v_dot(phit->normal, sray->direction)),
			sray->direction);
	r = v_normalize(r);
	v = v_normalize(v_sub(c->viewpoint, phit->hit_point));
	specular = powf(clampf(v_dot(r, v), 0, 1), 50);
	spec.r = light_col.r * specular * l->ratio;
	spec.g = light_col.g * specular * l->ratio;
	spec.b = light_col.b * specular * l->ratio;
	*final = color_add(*final, spec);
}
