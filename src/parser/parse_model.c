#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line.h"


bool	get_model_size(char *file_name, int *p_size, int *m_size)
{
	int	fd;
	char	*line;

	fd = open(file_name, O_RDONLY);
	*p_size = 0;
	*m_size = 0;
	line = get_next_line(fd);
	while (line)
	{
		skip_spaces(&line);
		if (ft_strncmp(line, "p ", 2) == 0)
			p_size++;
		else (ft_strncmp(line, "t ", 2) == 0)
			m_size++;
		else if (*line && *line != '\n')
			return (false);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (*p_size == 0 || *m_size == 0)
		return (false);
	return (true);
}

static	bool get_point(char *str, float *points, int i)
{
	next_info(&str);
	if (!is_correct_coordinate(str))
		return (false);

}

bool	load_object(int	fd, t_mesh *mesh)
{
	char	*line;
	int	p_index;
	int	t_index;

	p_index = 0;
	t_index = 0;
	line = get_next_line(fd);
	while (line)
	{
		skip_spaces(&line);
		if (ft_strncmp(line, "p ", 2) == 0 && !get_coordinates(line, mesh->points)
			
		else (ft_strncmp(line, "t ", 2) == 0)
			m_size++;
	}
}

bool	parse_model(char *model_data, t_obj *model)
{
	char	*file_name;
	int	fd;
	int	p_size;
	
	obj->type = MODEL;
	next_info(&model_data);
	mode->mesh = ft_calloc(sizeof(t_mesh), 1);
	if (!model->mesh)
		return (print_err("Can't allocate memory"));
	if (!get_coordinates(model_data, &model->center))
		return (free_obj(model), print_err("Model has invalid coordinates"));
	next_info(&model_data);
	if (!get_file_name(*model_data, &file_name))
		return (print_err("Model has invalid file name"));
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (print_err("File not exitsts or permission error"));
	if (!get_model_size(file_name, &p_size, &model->mesh->size))
		return (print_err(".obj file contains invalid symbols"));
	model->mesh->triangles = malloc(sizeof(int) * mode->mesh->size * 3);
	if (!mode->mehs->triangles)
		return (print_err("Can't allocate memory");
	model->mesh->points = malloc(sizeof(float) * p_size * 3);
	if (!model->mesh->points)
		return (print_err("Can't allocate memory");
	
	if (!get_color(model_data, &model->color))
		return (free_obj(model, print_err("Model has invalid color")));
	next_info(&model_data);

}
