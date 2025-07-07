#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include "parser.h"
#include "get_next_line.h"

int	parse_obj(char *obj_data)
{
	(void)obj_data;
	return (0);
}

bool	parse_ambient(char *light_data, t_scene *scene)
{
	t_amb_light	*amb;

	amb = malloc(sizeof(t_amb_light));
	if (!amb)
		return (false);
	light_data++;
	if (!get_ration(light_data, &amb->ratio))
		return (free(amb), false);
	skip_info(&light_data);
	if (!get_color(light_data, &amb->color))
		return (free(amb), false);
	skip_info(&light_data);
	skip_spaces(&light_data);
	if (*light_data || *light_data == '\n')
		return (false);
	scene->amb = amb;
	return (true);
}

int	parse_light(char *light_data)
{
	(void)light_data;
	return (0);
}

bool	parse_camera(char *camera_data, t_scene *scene)
{
	t_cam	*cam;

	cam = malloc(sizeof(t_cam));
	if (!cam)
		return (false);
	camera_data++;
	if (!get_coordinates(camera_data, &cam->view_point))
		return (free(cam), false);
	skip_info(&camera_data);
	if (!get_orientation(camera_data, &cam->orient_v))
		return (free(cam), false);
	skip_info(&camera_data);
	if (!get_fov(camera_data, &cam->fov))
		return (free(cam), false);
	skip_info(&camera_data);
	skip_spaces(&camera_data);
	if (*camera_data || *camera_data == '\n')
		return (false);
	scene->cam = cam;
	return (true);
}

t_scene		*load_scene(char *file_name)
{
	t_scene	*scene;
	char	*line;
	int	fd;

	if (!is_valid_name(file_name))
		return (NULL);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (NULL);
	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	line = get_next_line(fd);
	while (line)
	{
		skip_spaces(&line);
		if (*line || *line != '\n')
		{
			if (line[0] == 'A')
				if (!parse_ambient(line, scene))
					return (NULL);
			else if (line[0] == 'C')
				if (!parse_camera(line, scene))
					return (NULL);
			else
				printf("UNKONW TYPE )))\n");
		}
		free(line);
		line = get_next_line(fd);
	}
	return (scene);	
}
