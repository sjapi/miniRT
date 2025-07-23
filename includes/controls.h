/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:57:20 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 00:40:35 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROLS_H
# define CONTROLS_H

# include "minirt.h"
# include <stdbool.h>

/* key based funcs */
bool	handle_qweasd(int key, t_rt *info);
bool	handle_hjkl(int key, t_rt *info);
bool	handle_plus_minus(int key, t_rt *info);
bool	handle_xyz(int key, t_rt *info);
bool	handle_other_keys(int key, t_rt *info);

/* mode based funcs */
bool	translate_obj(t_obj *obj, int key);
bool	translate_cam(t_cam *cam, int key);
bool	translate_lights(t_light *lights, int lights_count, int key);

bool	rotate_obj(t_obj *obj, int key);
bool	rotate_cam(t_cam *cam, int key);
bool	resize_obj(t_obj *obj, int key);

bool	change_ambient(t_amb_light *amb, int key);

/* other */
t_obj	*mouse_click_obj(int button, int x, int y, t_rt *info);
void	select_obj(t_obj *obj, t_scene *scene);
void	deselect_obj(t_scene *scene);

bool	adjust_sphere(t_obj *obj, int key);
bool	adjust_cylinder(t_obj *obj, int key);
bool	adjust_cone(t_obj *obj, int key);

#endif
