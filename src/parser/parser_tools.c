#include <stdbool.h>
#include "minirt.h"
#include "parser.h"
#include "utils.h"

bool	get_color(char *obj_data, int *color_code)
{
	int	n;

	skip_spaces(&obj_data);
	if (!is_correct_color(obj_data))
		return (false);
	*color_code = 0;
	n = 0;
	while (*obj_data >= '0' && *obj_data <= '9')
		n = n * 10 + (*obj_data++ - '0');
	*color_code = n;
	n = 0;
	obj_data++;
	while (*obj_data >= '0' && *obj_data <= '9')
		n = n * 10 + (*obj_data++ - '0');
	*color_code = (*color_code << 8) | n;
	n = 0;
	obj_data++;
	while (*obj_data >= '0' && *obj_data <= '9')
		n = n * 10 + (*obj_data++ - '0');
	*color_code = (*color_code << 8) | n;
	return (true);
}

bool	get_coordinates(char *data, t_point3 *p) 
{
	skip_spaces(&data);
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

bool	get_orientation(char *data, t_point3 *o) 
{
	skip_spaces(&data);
	if (!is_correct_coordinate(data))
		return (false);
	o->x = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	o->y = ft_atof(data);
	while (*data && *data != ',')
		data++;
	data++;
	o->z = ft_atof(data);
	if (o->x < -1.0 || o->x > 1.0 
		|| o->y < -1.0 || o->y > 1.0 
		|| o->z < -1.0 || o->z > 1.0)
		return (false);
	return (true);
}

bool	get_ratio(char *data, float *ratio)
{
	skip_spaces(&data);
	if (!is_float(data))
		return (false);
	*ratio = ft_atof(data);
	if (*ratio < 0.0 || *ratio > 1.0)
		return (false);
	return (true);	
}

bool	get_fov(char *data, unsigned char *fov)
{
	skip_spaces(&data);
	if (!is_int(data))
		return (false);
	*fov = ft_atoi(data);
	if (*fov < 0 || *fov > 180)
		return (false);
	return (true);
}
