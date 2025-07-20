/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 21:06:54 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/20 17:00:49 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mlx.h"
#include "minirt.h"
#include "parser.h"
#include "defines.h"
#include "renderer.h"
#include "utils.h"
#include "controls.h"

static int	destroy(t_rt *info)
{
	free_rt(info);
	exit(0);
}

static int	handle_key_hooks(int key, t_rt *info)
{
	t_cam	*cam;
	bool	rerender;

	cam = info->scene->cam;
	if (key == KEY_ESC)
		return (destroy(info), 0);
	else if (match(key, (int []){KEY_Q, KEY_W, KEY_E, KEY_S, KEY_A, KEY_D}, 6))
		rerender = handle_qweasd(key, info);
	else if (match(key, (int []){KEY_H, KEY_J, KEY_K, KEY_L}, 4))
		rerender = handle_hjkl(key, info);
	else if (match(key, (int []){KEY_TOP, KEY_BOTTOM, KEY_LEFT, KEY_RIGHT}, 4))
		rerender = handle_hjkl(key, info);
	else if (key == KEY_MINUS || key == KEY_PLUS)
		rerender = handle_plus_minus(key, info);
	else if (key == KEY_X || key == KEY_Y || key == KEY_Z)
		rerender = handle_xyz(key, info);
	else
		rerender = handle_other_keys(key, info);
	if (rerender)
		render(info);
	return (0);
}

static int	handle_mouse_hook(int button, int x, int y, t_rt *info)
{
	t_obj	*obj;
	t_obj	*selected;

	obj = mouse_click_obj(button, x, y, info);
	selected = info->scene->selected;
	if (obj != NULL && selected != NULL && obj->id == selected->id)
		return (0);
	else if (obj != NULL && selected != NULL && obj->id != selected->id)
	{
		deselect_obj(info->scene);
		select_obj(obj, info->scene);
	}
	else if (obj != NULL && selected == NULL)
	{
		select_obj(obj, info->scene);
		info->mode = OBJECT_MODE;
	}
	else if (obj == NULL && selected != NULL)
	{
		deselect_obj(info->scene);
		info->mode = RENDER_MODE;
	}
	else
		return (0);
	return (render(info), 0);
}

int	main(int argc, char **argv)
{
	t_rt	info;

	if (argc != 2)
		return (printf("miniRT: wrong arguments count\n"), 1);
	if (!init_rt(&info, argv[1]))
		return (1);
	render(&info);
	mlx_hook(info.win, 2, 1L >> 0, handle_key_hooks, &info);
	mlx_mouse_hook(info.win, handle_mouse_hook, &info);
	mlx_hook(info.win, 17, 0, destroy, &info);
	mlx_loop(info.mlx);
	return (0);
}
