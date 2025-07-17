/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:01:55 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/17 19:42:55 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include "parser.h"
#include "utils.h"
#include "get_next_line.h"

bool	print_err(char *msg)
{
	printf("Error\n");
	printf("miniRT: parser: %s\n", msg);
	return (false);
}

void	free_obj(t_obj *obj)
{
	if (obj->attrs)
		free(obj->attrs);
	if (obj->texture)
		free(obj->texture);
	free(obj);
}

bool	append_obj(t_scene *scene, t_obj *obj)
{
	t_obj	*new_objs;

	new_objs = malloc(sizeof(t_obj) * (scene->objs_count + 1));
	if (!new_objs)
		return (false);
	if (scene->objs_count > 0)
		ft_memcpy(new_objs, scene->objs, sizeof(t_obj) * scene->objs_count);
	obj->id = scene->objs_count;
	new_objs[scene->objs_count] = *obj;
	free(scene->objs);
	scene->objs = new_objs;
	scene->objs_count++;
	return (true);
}

bool	append_light(t_scene *scene, t_light *light)
{
	t_light	*new_lights;

	new_lights = malloc(sizeof(t_light) * (scene->lights_count + 1));
	if (!new_lights)
		return (false);
	if (scene->lights_count > 0)
		ft_memcpy(new_lights, scene->lights, sizeof(t_light) * scene->lights_count);
	new_lights[scene->lights_count] = *light;
	free(scene->lights);
	scene->lights = new_lights;
	scene->lights_count++;
	return (true);
}



bool	get_line(int fd, char **line)
{
	if (*line)
		free(*line);
	*line = get_next_line(fd);
	if (!*line)
		return (false);
	return (true);
}

t_scene	*load_scene(char *file_name)
{
	t_scene	*scene;
	char	*line;
	int		fd;

	if (!is_valid_file(file_name, ".rt"))
		return (NULL);
	fd = open(file_name, O_RDONLY);
	scene = ft_calloc(sizeof(t_scene), 1);
	if (!scene)
		return (NULL);
	line = NULL;
	while (get_line(fd, &line))
	{
		skip_spaces(&line);
		if (!*line || *line == '\n' || *line == '#')
			continue ;
		if ((ft_strncmp(line, "A ", 2) == 0 && !parse_ambient(line, scene))
			|| (ft_strncmp(line, "C ", 2) == 0 && !parse_camera(line, scene))
			|| (ft_strncmp(line, "L ", 2) == 0 && !parse_light(line, scene))
			|| (ft_strncmp(line, "S ", 2) == 0 && !parse_skybox(line, scene))
			|| !parse_obj(line, scene))
			return (free(line), close(fd), NULL);
	}
	return (scene);
}
