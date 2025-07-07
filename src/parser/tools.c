#include "minirt.h"
#include "parser.h"

int	get_color(char *obj_data, int *num)
{
	int	color_code;
	int	i;

	obj_data = trim_spaces(obj_data);
	*num = 0;
	i = 0;
	while (*obj_data)
	{
		color_code = 0;
		while (*obj_data >= '0' && *obj_data <= '9')
			color_code = color_code * 10 + (*obj_data++ - '0');
		if (color_code < 0 || color_code > 255)
			return (0);
		*num = (*num << 8) | color_code;
		if (!*obj_data)
			break ;
		if (*obj_data != ',')
			return (0);
		i++;
	}
	if (i != 2)
		return (0);
	return (1);
}

int get_coordinates(char *obj_data, t_point3 *p)
{
	(void)obj_data;
	(void)p;
	return (0);
}
