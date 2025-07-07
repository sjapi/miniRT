/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */ /*   Created: 2025/07/01 16:20:31 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/07 15:53:24 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minirt.h"
#include "parser.h"
#include "defines.h"
#include "renderer.h"
#include "mlx.h"

/* MacOS does not have mlx_destoy_display func */
static int	destroy(void *param)
{
	t_rt	*info;
	
	info = (t_rt *)param;
	mlx_destroy_image(info->mlx, info->img);
	mlx_destroy_window(info->mlx, info->win);
//	mlx_destroy_display(info->mlx);
	free(info->mlx);
//	free_scene(info->scene);
	exit(0);
}

static bool	init_info(t_rt *info, char *filename)
{
//	info->scene = parser(filename);
//	if (!info->scene)
//	if (!parser(filename))
//		return (false);
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WIN_WIDTH, WIN_HEIGHT, "miniRT");
	info->img = mlx_new_image(info->mlx, WIN_WIDTH, WIN_HEIGHT);
	info->addr = mlx_get_data_addr(info->img, &info->bpp, &info->line_len, &info->endian);
	return (true);
}

static int	handle_key_hooks(int key, void *param)
{
	t_rt	*info;
	
	info = (t_rt *)param;
	printf("Key [%d] pressed.\n", key);
	return (0);
}

int	main(int argc, char **argv)
{
	t_rt	info;

	if (argc != 2)
		return (printf("miniRT: wrong arguments count\n"), 1);
	if (!init_info(&info, argv[1]))
		return (1);
//	info.scene->amb->ratio = 0.2;
//	info.scene->amb->color= 0xffffff;
//	render_scene(&info);
	mlx_hook(info.win, 2, 1L >> 0, handle_key_hooks, &info);
	mlx_hook(info.win, 17, 0, destroy, &info);
	mlx_loop(info.mlx);
	return (0);
}
