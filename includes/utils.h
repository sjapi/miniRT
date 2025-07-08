/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:23:26 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/08 13:41:07 by haaghaja         ###   ########.fr       */
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
float		ft_atof(char *str);


/* miniRT related */
bool	v_normalized(t_point3 p);
int		get_amb_color(t_amb_light *amb);

#endif
