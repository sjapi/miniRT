/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:21:07 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/18 16:33:58 by 032zolotarev     ###   ########.fr       */
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

