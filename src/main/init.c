/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:21:16 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/22 15:51:23 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "parser.h"
#include "defines.h"
#include "bounding.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static void	init_cam(t_cam *cam)
{
	cam->yaw = atan2f(cam->orient_v.x, cam->orient_v.z);
	cam->pitch = asinf(cam->orient_v.y);
	cam->t_fov = tanf((cam->fov * M_PI / 180.0f) / 2.0f);
}

static void	init_mlx(t_rt *info)
{
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WIN_WIDTH, WIN_HEIGHT, "miniRT");
	info->img = mlx_new_image(info->mlx, WIN_WIDTH, WIN_HEIGHT);
	info->addr = mlx_get_data_addr(info->img, &info->bpp, &info->line_len,
			&info->endian);
}

static void	init_skybox(t_rt *info)
{
	t_texture	*skybox;

	skybox = info->scene->skybox;
	skybox->mlx = mlx_xpm_file_to_image(info->mlx, skybox->file_name,
			&skybox->width, &skybox->height);
	if (!skybox->mlx)
	{
		print_err("Failed to load skybox");
		free_rt(info);
		exit(1);
	}
	info->scene->skybox->data = mlx_get_data_addr(skybox->mlx, &skybox->bpp,
			&skybox->line_length, &skybox->endian);
	info->scene->skybox->bpp = info->scene->skybox->bpp / 8;
}

static void	init_obj_textures(t_rt *info)
{
	int		i;
	t_obj	*obj;

	i = -1;
	while (++i < info->scene->objs_count)
	{
		obj = &info->scene->objs[i];
		if (obj->type != SPHERE || !obj->texture)
			continue ;
		obj->texture->mlx = mlx_xpm_file_to_image(info->mlx,
				obj->texture->file_name, &obj->texture->width,
				&obj->texture->height);
		if (!obj->texture->mlx)
		{
			print_err("Failed to object texture");
			free_rt(info);
			exit(1);
		}
		obj->texture->data = mlx_get_data_addr(obj->texture->mlx,
				&obj->texture->bpp, &obj->texture->line_length,
				&obj->texture->endian);
	}
}

bool	init_rt(t_rt *info, char *file_name)
{
	if (!load_scene(file_name, &info->scene))
		return (free_rt(info), print_err("Error"));
	if (!info->scene->cam || !info->scene->amb || info->scene->lights_count == 0)
		return (free_rt(info), printf("Error\n"), false);
	init_cam(info->scene->cam);
	info->mode = RENDER_MODE;
	info->win_aspect_ratio = (float)WIN_WIDTH / (float)WIN_HEIGHT;
	init_mlx(info);
	if (info->scene->skybox != NULL)
		init_skybox(info);
	init_obj_textures(info);
	info->scene->bvh = build_bvh(info->scene->objs, 0, info->scene->objs_count);
	return (true);
}
