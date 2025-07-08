#ifndef PARSER_H
# define PARSER_H

#include <stdbool.h>
#include "minirt.h"

/* parser.c */
t_scene	*load_scene(char *file_name);
bool	parse_camera(char *camera_data, t_scene *scene);
bool	parse_ambient(char *light_data, t_scene *scene);
bool	parse_light(char *light_data, t_scene *scene);

/* parser_utils.c */
bool	is_valid_name(char *file_name);
bool	is_whitespace(char c);
bool	is_correct_coordinate(char *str);
bool	is_correct_color(char *str);
bool	is_int(char *num);
bool	is_float(char *num);

void	skip_info(char **data);
void	skip_spaces(char **str);
bool	skip_integer(char **str, int *digits);

bool	parse_float(char **str, int *digits);

/* parser_tools.c */
bool	get_color(char *obj_data, int *num);
bool	get_coordinates(char *data, t_point3 *p); 
bool	get_orientation(char *data, t_point3 *o); 
bool	get_ratio(char *data, float *ratio);
bool	get_fov(char *data, unsigned char *fov);

#endif
