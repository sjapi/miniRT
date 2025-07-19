/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:43:16 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/19 19:58:14 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>
#include <stdio.h>
#include "mlx.h"

static void	free_texture(t_texture *t)
{
	free(t->mlx);
	free(t->file_name);
//	free(t->data);
	free(t);
}

static void	free_objects(t_obj *objs, int count)
{
	int		i;
	t_obj	*obj;

	i = 0;
	while (i < count)
	{
		obj = &objs[i];
		free(obj->attrs);
		if (obj->texture)
			free_texture(obj->texture);
		if (obj->mesh)
		{
			free(obj->mesh->file_name);
			free(obj->mesh->triangles);
			free(obj->mesh->points);
			free(obj->mesh);
		}
		i++;
	}
	free(objs);
}

static void	free_scene(t_scene *scene)
{
	free_objects(scene->objs, scene->objs_count);
	free(scene->amb);
	free(scene->cam);
	free(scene->lights);
	if (scene->skybox)
		free_texture(scene->skybox);
}

void	free_rt(t_rt *info)
{
	free(info->optim->viewport_x);
	free(info->optim->viewport_y);
	free(info->optim);
	free_scene(info->scene);

//	free(info->addr);
	mlx_destroy_image(info->mlx, info->img);
	mlx_destroy_window(info->mlx, info->win);
#ifdef __linux__
	mlx_destroy_display(info->mlx);
#endif
	free(info->mlx);
}
