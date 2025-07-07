#ifndef PARSER_H
# define PARSER_H

/* parser.c */
int		parser(char *file_name);

/* parser_utils.c */
int		is_valid_name(char *file_name);
int		is_white_space(char c);
char    *trim_spaces(char *str);

/* parser_tools.c */
int		get_color(char *obj_data, int *num);


#endif
