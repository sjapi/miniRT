/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:34:55 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/21 20:12:27 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_color	decrease_color(t_color color, int amount)
{
    color.r = (color.r > amount) ? color.r - amount : 0;
    color.g = (color.g > amount) ? color.g - amount : 0;
    color.b = (color.b > amount) ? color.b - amount : 0;
	return color;
}

t_color	reverse_color(t_color color)
{
	color.r = 255.0 - color.r;
	color.g = 255.0 - color.g;
	color.b = 255.0 - color.b;
	return (color);
}
