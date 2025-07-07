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

int	parse_ambient(char *light_data)
{
	(void)light_data;
	return (0);
}

int	parse_light(char *light_data)
{
	(void)light_data;
	return (0);
}

int	parse_camera(char *camera_data)
{
	(void)camera_data;
	return (0);
}

int	parser(char *file_name)
{
	char	*line;
	int		fd;

	if (!is_valid_name(file_name))
		return (false);
	fd = open(file_name, O_RDONLY);
	if (fd != -1)
		return (false);
	line = get_next_line(fd);
	while (line)
	{
		line = trim_spaces(line);
		if (!is_white_space(*line))
		{
			if (line[0] == 'A')
				parse_ambient(line);
			else if (line[0] == 'L')
				parse_light(line);
			else if (line[0] == 'C')
				parse_camera(line);
			else
				printf("UNKONW TYPE )))\n");
		}
		line = get_next_line(fd);
	}
	return (true);	
}
