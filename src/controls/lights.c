/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:21:07 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 14:04:21 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

bool	translate_lights(t_light *lights, int lights_count, int key)
{
	float	val;
	int		i;

	val = 0.2;
	i = 0;
	while (i < lights_count)
	{
		if (key == KEY_Q)
			lights[i].point.x += val;
		else if (key == KEY_A)
			lights[i].point.x -= val;
		else if (key == KEY_W)
			lights[i].point.y += val;
		else if (key == KEY_S)
			lights[i].point.y -= val;
		else if (key == KEY_E)
			lights[i].point.z += val;
		else if (key == KEY_D)
			lights[i].point.z -= val;
		else
			return (false);
		i++;
	}
	return (true);
}

bool	change_ambient(t_amb_light *amb, int key)
{
	if (key == KEY_O)
		amb->ratio = clampf(amb->ratio - 0.1, 0.1, 1.0);
	else if (key == KEY_P)
		amb->ratio = clampf(amb->ratio + 0.1, 0.1, 1.0);
	else
		return (false);
	return (true);
}
