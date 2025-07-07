#ifndef UTILS_H
# define UTILS_H

#include "minirt.h"
#include <stdbool.h>

/* libft */
int		ft_strlen(char *str);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);

/* miniRT related */
bool	v_normalized(t_point3 p);
int		get_amb_color(t_amb_light *amb);

#endif
