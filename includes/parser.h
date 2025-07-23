/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:00:29 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/23 19:17:31 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include "minirt.h"

/* parser.c */
bool	load_scene(char *file_name, t_scene **scene);
bool	print_err(char *msg);
bool	append_obj(t_scene *scene, t_obj *obj);
bool	append_light(t_scene *scene, t_light *light);
bool	get_line(int fd, char **line);

/* parser_utils.c */
int		count_digits(char *num);
bool	is_valid_file(char *file_name, char *ext);
bool	is_whitespace(char c);
bool	is_correct_coordinate(char *str);
bool	is_correct_color(char *str);
bool	is_int(char *num);
bool	is_float(char *num);
bool	is_checkerboard(char *str);
bool	is_mirror(char *str);
bool	next_info(char **data);
void	skip_spaces(char **str);
bool	skip_integer(char **str, int *digits);
bool	parse_float(char **str, int *digits);

/* parser_tools.c */
bool	get_color(char *obj_data, int *num);
bool	get_coordinates(char *data, t_vec3 *p);
bool	get_orientation(char *data, t_vec3 *o);
bool	get_ratio(char *data, float *ratio);
bool	get_fov(char *data, int *fov);
bool	get_attribute(char *data, float *d);
bool	get_file_name(char *obj_data, char **name);

/* parse_elements.c */
bool	parse_ambient(char *light_data, t_scene *scene);
bool	parse_light(char *light_data, t_scene *scene);
bool	parse_camera(char *camera_data, t_scene *scene);
bool	parse_skybox(char *skybox_data, t_scene *scene);
bool	parse_element(char *line, t_scene *scene);

/* parse_objects.c */
bool	parse_obj(char *obj_data, t_scene *scene);
bool	parse_cylinder(char *cylinder_data, t_obj *cylinder);
bool	parse_sphere(char *sphere_data, t_obj *sphere);
bool	parse_plane(char *plane_data, t_obj *plane);
bool	parse_cone(char *data, t_obj *cone);
bool	parse_model(char *model_data, t_obj *model);

/* model_utils.c */
bool	get_triangle(char *data, t_tri *triangle, t_vec3 *points, int ps);
bool	get_point(char *data, t_vec3 *p);
bool	get_model_size(char *file_name, int *p_size, int *m_size);

#endif
