/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:34:55 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/20 20:01:25 by azolotar         ###   ########.fr       */
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
