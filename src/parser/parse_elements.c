/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:01:21 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/22 15:17:25 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "utils.h"

bool	parse_ambient(char *light_data, t_scene *scene)
{
	t_amb_light	*amb;

	if (scene->amb)
		return (print_err("Scene can have only 1 amb"));
	amb = malloc(sizeof(t_amb_light));
	if (!amb)
		return (print_err("Can't allocate memory"));
	next_info(&light_data);
	if (!get_ratio(light_data, &amb->ratio))
		return (free(amb), print_err("Ambient has invalid ratio"));
	next_info(&light_data);
	if (!get_color(light_data, &amb->color))
		return (free(amb), print_err("Ambient has invalid color"));
	next_info(&light_data);
	if (*light_data && *light_data != '\n')
		return (free(amb), print_err("Ambient has invalid data"));
	scene->amb = amb;
	return (true);
}

bool	parse_light(char *light_data, t_scene *scene)
{
	t_light	*light;

	light = malloc(sizeof(t_light));
	if (!light)
		return (print_err("Can't allocate memory"));
	next_info(&light_data);
	if (!get_coordinates(light_data, &light->point))
		return (free(light), print_err("Light has invalid coordinates"));
	next_info(&light_data);
	if (!get_ratio(light_data, &light->ratio))
		return (free(light), print_err("Light has invalid ratio"));
	next_info(&light_data);
	if (!get_color(light_data, &light->color))
		return (free(light), print_err("Light has invalid color"));
	next_info(&light_data);
	if (*light_data && *light_data != '\n')
		return (free(light), print_err("Light has invalid data"));
	append_light(scene, light);
	return (true);
}

bool	parse_skybox(char *skybox_data, t_scene *scene)
{
	t_texture	*skybox;

	if (scene->skybox)
		return (print_err("Scene can have only 1 skybox"));
	skybox = malloc(sizeof(t_texture));
	if (!skybox)
		return (print_err("Can't allocate memory"));
	next_info(&skybox_data);
	if (!get_file_name(skybox_data, &skybox->file_name))
		return (print_err("Skybox has invalid name"));
	if (!is_valid_file(skybox->file_name, ".xpm"))
		return (free(skybox->file_name), print_err("Skybox has invalid file"));
	next_info(&skybox_data);
	if (*skybox_data && *skybox_data != '\n')
		return (free(skybox->file_name), print_err("Skybox has invalid data"));
	scene->skybox = skybox;
	return (true);
}

bool	parse_camera(char *camera_data, t_scene *scene)
{
	t_cam	*cam;

	if (scene->cam)
		return (print_err("Scene can have only 1 camera"));
	cam = malloc(sizeof(t_cam));
	if (!cam)
		return (print_err("Can't allocate memory"));
	next_info(&camera_data);
	if (!get_coordinates(camera_data, &cam->viewpoint))
		return (free(cam), print_err("Camera has invalid coordinates"));
	next_info(&camera_data);
	if (!get_orientation(camera_data, &cam->orient_v))
		return (free(cam), print_err("Camera has invalid orientation"));
	next_info(&camera_data);
	if (!get_fov(camera_data, &cam->fov))
		return (free(cam), print_err("Camera has invalid fov"));
	next_info(&camera_data);
	if (*camera_data && *camera_data != '\n')
		return (free(cam), print_err("Camera has invalid data"));
	scene->cam = cam;
	return (true);
}

bool	parse_element(char *line, t_scene *scene)
{
	skip_spaces(&line);
	if (!*line || *line == '\n' || *line == '#')
		return (true);
	if (!((ft_strncmp(line, "A ", 2) == 0 && parse_ambient(line, scene))
			|| (ft_strncmp(line, "C ", 2) == 0 && parse_camera(line, scene))
			|| (ft_strncmp(line, "L ", 2) == 0 && parse_light(line, scene))
			|| (ft_strncmp(line, "S ", 2) == 0 && parse_skybox(line, scene))
			|| parse_obj(line, scene)))
		return (false);
	return (true);
}
