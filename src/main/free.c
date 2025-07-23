/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:43:16 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/24 00:45:10 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>
#include <stdio.h>
#include "mlx.h"

static void	my_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

static void	free_texture(t_rt *info, t_texture *t)
{
	if (t->mlx)
		mlx_destroy_image(info->mlx, t->mlx);
	my_free(t->file_name);
	my_free(t);
}

static void	free_objects(t_rt *info, t_obj *objs, int count)
{
	int		i;
	t_obj	*obj;

	i = 0;
	while (i < count)
	{
		obj = &objs[i];
		my_free(obj->attrs);
		if (obj->texture)
			free_texture(info, obj->texture);
		if (obj->mesh)
		{
			my_free(obj->mesh->file_name);
			my_free(obj->mesh->triangles);
			my_free(obj->mesh->points);
			my_free(obj->mesh);
		}
		i++;
	}
	free(objs);
}

static void	free_scene(t_rt *info, t_scene *scene)
{
	if (!scene)
		return ;
	free_objects(info, scene->objs, scene->objs_count);
	my_free(scene->amb);
	my_free(scene->cam);
	my_free(scene->lights);
	if (scene->skybox)
		free_texture(info, scene->skybox);
	my_free(scene);
}

void	free_rt(t_rt *info)
{
	if (info->optim)
	{
		my_free(info->optim->viewport_x);
		my_free(info->optim->viewport_y);
		my_free(info->optim);
	}
	free_scene(info, info->scene);
	if (!info->mlx)
		return ;
	mlx_destroy_image(info->mlx, info->img);
	mlx_destroy_window(info->mlx, info->win);
	mlx_destroy_display(info->mlx);
	my_free(info->mlx);
}
