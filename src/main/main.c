/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:10:28 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/08 15:53:21 by azolotar         ###   ########.fr       */
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

// ========================================================================

void print_point(t_point3 p)
{
    printf("(%.2f, %.2f, %.2f)", p.x, p.y, p.z);
}

void print_scene(t_scene *scene)
{
    // Ambient light
    if (scene->amb)
    {
        printf("amb: ratio=%.2f color=#%06X\n", scene->amb->ratio, scene->amb->color);
    }

    // Camera
    if (scene->cam)
    {
        printf("cam: view_point=");
        print_point(scene->cam->view_point);
        printf(" orient_v=");
        print_point(scene->cam->orient_v);
        printf(" fov=%u\n", scene->cam->fov);
    }

    // Lights
    for (int i = 0; i < scene->lights_count; i++)
    {
        printf("light %d: point=", i + 1);
        print_point(scene->lights[i].point);
        printf(" ratio=%.2f color=#%06X\n", scene->lights[i].ratio, scene->lights[i].color);
    }

    // Objects
    for (int i = 0; i < scene->objs_count; i++)
    {
        t_obj *obj = &scene->objs[i];
        printf("obj %d: ", i + 1);
        switch (obj->type)
        {
            case 1: printf("square"); break;
            case 2: printf("sphere"); break;
            case 3: printf("plane"); break;
            default: printf("unknown"); break;
        }
        printf(" center=");
        print_point(obj->center);
        printf(" color=#%06X", obj->color);
        if (obj->type != 2) // not sphere -> has norm vector
        {
            printf(" norm=");
            print_point(obj->norm_vector);
        }
        printf("\n");
    }
}

// ========================================================================








void	mock_init(t_rt *info)
{
	info->scene = malloc(sizeof(t_scene));
	info->scene->objs = NULL;
	info->scene->objs_count = 0;
	info->scene->amb = NULL;
	info->scene->cam = NULL;
	info->scene->lights = NULL;
	info->scene->lights_count = 0;
}

static void	free_scene(t_scene *scene)
{
	(void)scene;
}

/* MacOS does not have mlx_destoy_display func */
static int	destroy(void *param)
{
	t_rt	*info;
	
	info = (t_rt *)param;
	mlx_destroy_image(info->mlx, info->img);
	mlx_destroy_window(info->mlx, info->win);
//	mlx_destroy_display(info->mlx);
	free(info->mlx);
	free_scene(info->scene);
	exit(0);
}

static bool	init_info(t_rt *info, char *filename)
{
//	info->scene = parser(filename);
//	if (!info->scene)
	mock_init(info);
	info->win_aspect_ratio = (float)WIN_WIDTH / (float)WIN_HEIGHT;
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
	info.scene = load_scene(argv[1]);
	//print_scene(info.scene);
	render_scene(&info);
	mlx_hook(info.win, 2, 1L >> 0, handle_key_hooks, &info);
	mlx_hook(info.win, 17, 0, destroy, &info);
	mlx_loop(info.mlx);
	return (0);
}
