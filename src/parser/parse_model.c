/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_model.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:13:39 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/23 17:47:02 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "get_next_line.h"
#include "parser.h"
#include "utils.h"
#include "defines.h"

bool	parse_mesh(char *line, t_mesh *mesh, int *pi, int *ti)
{
	skip_spaces(&line);
	if (ft_strncmp(line, "p ", 2) == 0)
	{
		next_info(&line);
		if (!get_point(line, &mesh->points[*pi]))
			return (false);
		(*pi)++;
	}
	else if (ft_strncmp(line, "t ", 2) == 0)
	{
		next_info(&line);
		if (!get_triangle(line, &mesh->triangles[*ti], mesh->points, mesh->ps))
			return (false);
		(*ti)++;
	}
	next_info(&line);
	if (*line && *line != '\n')
		return (false);
	return (true);
}

bool	load_model(int fd, t_obj *model)
{
	char	*line;
	int		pi;
	int		ti;
	t_mesh	*mesh;

	mesh = model->mesh;
	pi = 0;
	ti = 0;
	line = NULL;
	if (!get_model_size(mesh->file_name, &mesh->ps, &mesh->size))
		return (print_err(".obj file contains invalid symbols"));
	mesh->triangles = malloc(sizeof(t_tri) * mesh->size);
	if (!mesh->triangles)
		return (print_err("Can't allocate memory"));
	mesh->points = malloc(sizeof(t_vec3) * mesh->ps);
	if (!mesh->points)
		return (print_err("Can't allocate memory"));
	while (get_line(fd, &line))
		if (!parse_mesh(line, mesh, &pi, &ti))
			return (free(line), false);
	pi = -1;
	while (++pi < mesh->ps)
		mesh->points[pi] = v_add(mesh->points[pi], model->center);
	return (true);
}

bool	parse_model(char *model_data, t_obj *model)
{
	int		fd;

	model->type = MODEL;
	next_info(&model_data);
	model->mesh = ft_calloc(sizeof(t_mesh), 1);
	if (!model->mesh)
		return (print_err("Can't allocate memory"));
	if (!get_coordinates(model_data, &model->center))
		return (print_err("Model has invalid coordinates"));
	next_info(&model_data);
	if (!get_file_name(model_data, &model->mesh->file_name))
		return (print_err("Model has invalid file name"));
	fd = open(model->mesh->file_name, O_RDONLY);
	if (fd == -1)
		return (print_err("File not exitsts or permission error"));
	if (!load_model(fd, model))
		return (close(fd), print_err("Can't load the model"));
	close(fd);
	next_info(&model_data);
	if (!get_color(model_data, &model->color))
		return (print_err("Model has invalid color"));
	next_info(&model_data);
	if (*model_data && *model_data != '\n')
		return (print_err("Model has invalid data"));
	return (true);
}
