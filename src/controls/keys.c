/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:54:45 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 13:38:42 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "defines.h"
#include "controls.h"
#include <math.h>
#include <stdbool.h>

bool	handle_qweasd(int key, t_rt *info)
{
	t_obj	*obj;

	if (info->mode == OBJECT_MODE && obj != NULL)
	{
		obj = info->scene->selected;
		return (translate_obj(obj, key));
	}
	else if (info->mode == CAMERA_MODE)
	{
		return (translate_cam(info->scene->cam, key));
	}
	else if (info->mode == LIGHT_MODE)
	{
		return (translate_lights(info->scene->lights,
				info->scene->lights_count, key));
	}
	else
	{
		return (false);
	}
}

bool	handle_hjkl(int key, t_rt *info)
{
	if (info->mode == RENDER_MODE && key == KEY_L)
	{
		info->mode = LIGHT_MODE;
		return (true);
	}
	else if (info->mode == LIGHT_MODE && key == KEY_L)
	{
		info->mode = RENDER_MODE;
		return (true);
	}
	else if (info->mode == CAMERA_MODE)
	{
		return (rotate_cam(info->scene->cam, key));
	}
	else
	{
		return (false);
	}
}

bool	handle_plus_minus(int key, t_rt *info)
{
	t_cam	*cam;

	if (info->mode == CAMERA_MODE)
	{
		cam = info->scene->cam;
		if (key == KEY_MINUS)
			cam->fov = clamp(cam->fov - 1, 1, 179);
		else if (key == KEY_PLUS)
			cam->fov = clamp(cam->fov + 1, 1, 179);
		else
			return (false);
		cam->t_fov = tanf((cam->fov * M_PI / 180.0f) / 2.0f);
		return (true);
	}
	else
	{
		return (false);
	}
}

// rotation
bool	handle_xyz(int key, t_rt *info)
{
	t_obj	*obj;

	obj = info->scene->selected;
	if (info->mode == OBJECT_MODE && obj != NULL)
	{
		return (rotate_obj(obj, key));
	}
	else
	{
		return (false);
	}
}

bool	handle_other_keys(int key, t_rt *info)
{
	bool	render;

	render = true;
	if (key == KEY_R && info->mode != RENDER_MODE)
	{
		if (info->mode == OBJECT_MODE && info->scene->selected != NULL)
			deselect_obj(info->scene);
		info->mode = RENDER_MODE;
	}
	else if (key == KEY_C && info->mode != CAMERA_MODE)
		info->mode = CAMERA_MODE;
	else if (key == KEY_C && info->mode == CAMERA_MODE)
		info->mode = RENDER_MODE;
	else if (match(key, (int [])
			{KEY_TOP, KEY_BOTTOM, KEY_LEFT, KEY_RIGHT}, 4))
	{
		if (info->mode == OBJECT_MODE && info->scene->selected != NULL)
			resize_obj(info->scene->selected, key);
	}
	else if (info->mode == LIGHT_MODE && (key == KEY_O || key == KEY_P))
		change_ambient(info->scene->amb, key);
	else
		render = false;
	return (render);
}
