/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:26:53 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 16:04:11 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOUNDING_H
# define BOUNDING_H

# include <stdbool.h>
# include "minirt.h"

typedef struct s_bvh_node
{
	t_vec3				aabb_min;
	t_vec3				aabb_max;
	struct s_bvh_node	*left;
	struct s_bvh_node	*right;
	t_obj				*object;
}	t_bvh_node;

/* bvh.c */
t_bvh_node	*build_bvh(t_obj *objects, int start, int end);
void		sort_by_axis(t_obj *objects, int count, int axis);

/* calculation.c */
void		calculate_aabb(t_obj *model);

/* bvh_utils.c */
t_vec3		v_min(t_vec3 *v0, t_vec3 *v1);
t_vec3		v_max(t_vec3 *v0, t_vec3 *v1);
void		find_selected(t_scene *scene);
void	free_bvh(t_bvh_node *bvh);
#endif
