#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line.h"
#include "parser.h"
#include "utils.h"
#include "defines.h"


static bool	get_model_size(char *file_name, int *p_size, int *m_size)
{
	int	fd;
	char	*line;
	char	*pline;
	fd = open(file_name, O_RDONLY);
	*p_size = 0;
	*m_size = 0;
	line = get_next_line(fd);
	while (line)
	{
		pline = line;
		skip_spaces(&line);
		if (ft_strncmp(line, "p ", 2) == 0)
			(*p_size)++;
		else if (ft_strncmp(line, "t ", 2) == 0)
			(*m_size) += 3;
		else if (*line && *line != '\n')
			return (false);
		free(pline);
		line = get_next_line(fd);
	}
	close(fd);
	if (*p_size == 0 || *m_size == 0)
		return (false);
	return (true);
}

static	bool get_point(char *data, t_vec3 *p)
{
	if (!is_correct_coordinate(data))
		return (false);
	p->x = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	p->y = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	p->z = ft_atof(data);
	return (true);
	
}

static	bool get_triangle(char *data, int *triangle, int i)
{
	if (!is_correct_coordinate(data))
		return (false);
	triangle[i] = ft_atoi(data);
	printf("%d ", triangle[i]);
	while (*data && *data != ',')
		data++;
	data++;
	triangle[i + 1] = ft_atoi(data);
	printf("%d ", triangle[i + 1]);
	while (*data && *data != ',')
		data++;
	data++;
	triangle[i + 2] = ft_atoi(data);
	printf("%d\n", triangle[i + 2]);
	return (true);
	
}

bool	load_model(int	fd, t_mesh *mesh)
{
	char	*line;
	char	*pline;
	int	p_index;
	int	t_index;

	p_index = 0;
	t_index = 0;
	line = get_next_line(fd);
	while (line)
	{
		pline = line;
		skip_spaces(&line);
		if (ft_strncmp(line, "p ", 2) == 0)
		{
			next_info(&line);
			if (!get_point(line, &mesh->points[p_index]))
				return (free(pline), false);
			p_index++;
		}
		else if (ft_strncmp(line, "t ", 2) == 0)
		{
			next_info(&line);
			if (!get_triangle(line, mesh->triangles, t_index))
				return (free(pline), false);
			t_index += 3;
		}
		next_info(&line);
		if (*line && *line != '\n')
			return (free(pline), false);
		free(pline);
		line = get_next_line(fd);
	}
	return (true);
}

bool	parse_model(char *model_data, t_obj *model)
{
	char	*file_name;
	int	fd;
	int	p_size;
	
	model->type = MODEL;
	next_info(&model_data);
	model->mesh = ft_calloc(sizeof(t_mesh), 1);
	if (!model->mesh)
		return (print_err("Can't allocate memory"));
	if (!get_coordinates(model_data, &model->center))
		return (free_obj(model), print_err("Model has invalid coordinates"));
	next_info(&model_data);
	if (!get_file_name(model_data, &file_name))
		return (print_err("Model has invalid file name"));
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (print_err("File not exitsts or permission error"));
	if (!get_model_size(file_name, &p_size, &model->mesh->size))
		return (print_err(".obj file contains invalid symbols"));
	model->mesh->triangles = malloc(sizeof(int) * model->mesh->size);
	if (!model->mesh->triangles)
		return (print_err("Can't allocate memory"));
	model->mesh->points = malloc(sizeof(t_vec3) * p_size);
	if (!model->mesh->points)
		return (print_err("Can't allocate memory"));	
	if (!load_model(fd, model->mesh))
		return (close(fd), print_err("Can't load the model"));
	next_info(&model_data);
	if (!get_color(model_data, &model->color))
		return (free_obj(model), print_err("Model has invalid color"));
	next_info(&model_data);
	if (*model_data && *model_data != '\n')
		return (free_obj(model), print_err("Model has invalid data"));
	close(fd);
	return (true);
}
