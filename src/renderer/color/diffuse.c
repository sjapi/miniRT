/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:45:56 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 13:51:09 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "utils.h"

t_color	compute_diffuse(t_hit *phit, t_ray *sray, t_light *l, t_color *obj_col)
{
	t_color	light_col;
	t_color	diffuse_col;
	float	diffuse;

	light_col = int_to_color(l->color);
	diffuse = v_dot(phit->normal, sray->direction);
	diffuse = clampf(diffuse, 0, 1);
	diffuse_col.r = obj_col->r * light_col.r / 255.0f * diffuse * l->ratio;
	diffuse_col.g = obj_col->g * light_col.g / 255.0f * diffuse * l->ratio;
	diffuse_col.b = obj_col->b * light_col.b / 255.0f * diffuse * l->ratio;
	return (diffuse_col);
}
