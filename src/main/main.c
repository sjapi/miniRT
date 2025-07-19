/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*   Created: 2025/07/01 16:20:31 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/08 16:56:38 by haaghaja         ###   ########.fr       */
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

static void	init_helpers(t_rt *info)
{
	t_cam	*cam;

	cam = info->scene->cam;
	cam->yaw = atan2f(cam->orient_v.x, cam->orient_v.z);
	cam->pitch = asinf(cam->orient_v.y);
}

void	free_rt(t_rt *info);

static int	destroy(t_rt *info)
{
	free_rt(info);
	exit(0);
}

static bool	init_info(t_rt *info, char *file_name)
{
	info->win_aspect_ratio = (float)WIN_WIDTH / (float)WIN_HEIGHT;
	info->scene = load_scene(file_name);
	if (!info->scene)
		return (false);
	init_helpers(info);
	info->mode = RENDER_MODE;
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WIN_WIDTH, WIN_HEIGHT, "miniRT");
	info->img = mlx_new_image(info->mlx, WIN_WIDTH, WIN_HEIGHT);
	info->addr = mlx_get_data_addr(info->img, &info->bpp, &info->line_len, &info->endian);

	if (!info->scene->skybox)
		return (true);
	info->scene->skybox->mlx = mlx_xpm_file_to_image(
		info->mlx, info->scene->skybox->file_name,
		&info->scene->skybox->width,
		&info->scene->skybox->height
	);
	if (!info->scene->skybox->mlx)
	{
		printf("Failed to load skybox\n");
		exit(1);
	}
	info->scene->skybox->data = mlx_get_data_addr(
		info->scene->skybox->mlx,
		&info->scene->skybox->bpp,
		&info->scene->skybox->line_length,
		&info->scene->skybox->endian
	);
	// ===================  OBJ ========================
	int	i = -1;
	while (i++ < info->scene->objs_count)
	{
		t_obj *obj = &info->scene->objs[i];
		if (obj->type != SPHERE || !obj->texture)
			continue ;
		obj->texture->mlx = mlx_xpm_file_to_image(
			info->mlx, obj->texture->file_name,
			&obj->texture->width,
			&obj->texture->height
		);
		if (!obj->texture->mlx)
		{
			printf("Failed to load texture\n");
			exit(1);
		}
		obj->texture->data = mlx_get_data_addr(
			obj->texture->mlx,
			&obj->texture->bpp,
			&obj->texture->line_length,
			&obj->texture->endian
		);
	}
	// =================================================
	return (true);
}

static int	handle_key_hooks(int key, t_rt *info)
{
	t_cam	*cam;
	bool	render;
	
	printf("key %d\n", key);
	cam = info->scene->cam;
	if (key == KEY_ESC)
		return (destroy(info), 0);
	else if (key == KEY_Q || key == KEY_W || key == KEY_E || key == KEY_S || key == KEY_A || key == KEY_D)
		render = handle_qweasd(key, info);
	else if (key == KEY_H || key == KEY_J || key == KEY_K || key == KEY_L)
		render = handle_hjkl(key, info);
	else if (key == KEY_MINUS || key == KEY_PLUS)
		render = handle_plus_minus(key, info);
	else if (key == KEY_X || key == KEY_Y || key == KEY_Z)
		render = handle_xyz(key, info);
	else
		render = handle_other_keys(key, info);
	if (render)
		render_scene(info);
	return (0);
}

t_obj	*mouse_click_obj(int button, int x, int y, t_rt *info);

int	handle_mouse_hook(int button, int x, int y, t_rt *info)
{
	t_obj	*obj;

	obj = mouse_click_obj(button, x, y, info);
	if (obj != NULL)
	{
		if (info->scene->selected != NULL)
		{
			if (info->scene->selected->id == obj->id)
				return (0);
			info->scene->selected->selected = false;
			info->scene->selected = NULL;
			printf("object deselected\n");
		}
		obj->selected = true;
		info->scene->selected = obj;
		printf("obj selected\n");
		if (info->mode != OBJECT_MODE)
			info->mode = OBJECT_MODE;
	}
	else
	{
		if (info->scene->selected != NULL)
		{
			info->scene->selected->selected = false;
			info->scene->selected = NULL;
			printf("object deselected\n");
			info->mode = RENDER_MODE;
		}
	}
	render_scene(info);
	return (0);
}

int	main(int argc, char **argv)
{
	t_rt	info;

	if (argc != 2)
		return (printf("miniRT: wrong arguments count\n"), 1);
	if (!init_info(&info, argv[1]))
		return (1);
	render_scene(&info);
	mlx_hook(info.win, 2, 1L >> 0, handle_key_hooks, &info);
	mlx_mouse_hook(info.win, handle_mouse_hook, &info);
	mlx_hook(info.win, 17, 0, destroy, &info);
	mlx_loop(info.mlx);
	return (0);
}
