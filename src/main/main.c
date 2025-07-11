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
#include "minirt.h"
#include "parser.h"
#include "defines.h"
#include "renderer.h"
#include "utils.h"
#include "mlx.h"
#include <math.h>

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
        print_point(scene->cam->viewpoint);
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
            case 4: printf("cylinder"); break;
            case 5: printf("cone"); break;
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
		if (obj->type == 2 || obj->type == 4)
			printf(" diameter=%.2f", obj->attrs[SPHERE_D_I]);
		if (obj->type == 4)
				printf(" height=%.2f", obj->attrs[CYLINDER_H_I]);
   	    printf("\n");
    }
}

// ========================================================================

static void	free_scene(t_scene *scene)
{
	(void)scene;
}

static void	init_helpers(t_rt *info)
{
	t_cam	*cam;

	cam = info->scene->cam;
	cam->yaw = atan2f(cam->orient_v.x, cam->orient_v.z);
	cam->pitch = asinf(cam->orient_v.y);
}

/* MacOS does not have mlx_destoy_display func */
static int	destroy(void *param)
{
	t_rt	*info;
	
	info = (t_rt *)param;
	mlx_destroy_image(info->mlx, info->img);
	mlx_destroy_window(info->mlx, info->win);
#ifdef __linux__
	mlx_destroy_display(info->mlx);
#endif
	free(info->mlx);
	free_scene(info->scene);
	exit(0);
}

static bool	init_info(t_rt *info, char *file_name)
{
	info->win_aspect_ratio = (float)WIN_WIDTH / (float)WIN_HEIGHT;
	info->scene = load_scene(file_name);
	if (!info->scene)
		return (false);
	init_helpers(info);
	info->mlx = mlx_init();
	info->win = mlx_new_window(info->mlx, WIN_WIDTH, WIN_HEIGHT, "miniRT");
	info->img = mlx_new_image(info->mlx, WIN_WIDTH, WIN_HEIGHT);
	info->addr = mlx_get_data_addr(info->img, &info->bpp, &info->line_len, &info->endian);

	info->scene->skybox = mlx_xpm_file_to_image(
		info->mlx, "skybox.xpm",
		&info->scene->skybox_width,
		&info->scene->skybox_height
	);
	if (!info->scene->skybox)
	{
		printf("Failed to load skybox\n");
		exit(1);
	}
	info->scene->skybox_data = mlx_get_data_addr(
		info->scene->skybox,
		&info->scene->skybox_bpp,
		&info->scene->skybox_line_length,
		&info->scene->skybox_endian
	);

	return (true);
}

bool	init_optimization(t_rt *info);

static int	handle_key_hooks(int key, void *param)
{
	t_rt	*info;
	t_cam	*cam;
	
	info = (t_rt *)param;
	cam = info->scene->cam;
	printf("Key [%d] pressed.\n", key);
	if (key == KEY_ESC)
		destroy(param);
	if (key == KEY_W)
	{
		t_point3 forward = cam->orient_v;
		cam->viewpoint = v_add(cam->viewpoint, v_scale(forward, 0.7));
	}
	if (key == KEY_S)
	{
		t_point3 forward = cam->orient_v;
		cam->viewpoint = v_sub(cam->viewpoint, v_scale(forward, 0.7));
	}
	if (key == KEY_A)
	{
		t_point3 right = v_normalize(v_cross((t_point3){0,1,0}, cam->orient_v));
		cam->viewpoint = v_sub(cam->viewpoint, v_scale(right, 0.7));
	}
	if (key == KEY_D)
	{
		t_point3 right = v_normalize(v_cross((t_point3){0,1,0}, cam->orient_v));
		cam->viewpoint = v_add(cam->viewpoint, v_scale(right, 0.7));
	}
	if (key == KEY_H)
		cam->yaw -= 0.1;
	if (key == KEY_L)
		cam->yaw += 0.1;
	if (key == KEY_J)
		cam->pitch += 0.1;
	if (key == KEY_K)
		cam->pitch -= 0.1;
	if (key == KEY_MINUS)
		cam->fov = clamp(cam->fov - 1, 1, 179);
	if (key == KEY_PLUS)
		cam->fov = clamp(cam->fov + 1, 1, 179);
	cam->pitch = clampf(cam->pitch, -1.55, 1.55);
	cam->orient_v = (t_point3){
		cosf(cam->pitch) * sinf(cam->yaw),
		sinf(cam->pitch),
		cosf(cam->pitch) * cosf(cam->yaw)
	};
	init_optimization(info);
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
	init_optimization(&info);
	render_scene(&info);
	print_scene(info.scene);
	mlx_hook(info.win, 2, 1L >> 0, handle_key_hooks, &info);
	mlx_hook(info.win, 17, 0, destroy, &info);
	mlx_loop(info.mlx);
	return (0);
}
