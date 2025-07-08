/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:23:26 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/08 18:06:55 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include "minirt.h"
#include <stdbool.h>

/* libft */
int		ft_strlen(char *str);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);
int		ft_atoi(char *str);
float	ft_atof(char *str);


/* miniRT related */
int			get_amb_color(t_amb_light *amb);

/* vectors */
bool		v_normalized(t_point3 p);
t_point3	v_add(t_point3 a, t_point3 b);
t_point3	v_scale(t_point3 v, float s);
t_point3	v_cross(t_point3 a, t_point3 b);
t_point3	v_normalize(t_point3 v);

#endif
