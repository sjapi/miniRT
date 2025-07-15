/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:02:45 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/15 19:12:56 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "minirt.h"
#include "utils.h"
#include <math.h>
#include <stdbool.h>

bool	translate_cam(t_cam *cam, int key)
{
	float	val;

	val = 0.4;
	if (key == KEY_Q)
		cam->viewpoint.x += val;
	else if (key == KEY_A)
		cam->viewpoint.x -= val;
	else if (key == KEY_W)
		cam->viewpoint.y += val;
	else if (key == KEY_S)
		cam->viewpoint.y -= val;
	else if (key == KEY_E)
		cam->viewpoint.z += val;
	else if (key == KEY_D)
		cam->viewpoint.z -= val;
	else
		return (false);
	return (true);
}

bool	rotate_cam(t_cam *cam, int key)
{
	if (key == KEY_H)
		cam->yaw -= 0.1;
	else if (key == KEY_L)
		cam->yaw += 0.1;
	else if (key == KEY_J)
		cam->pitch += 0.1;
	else if (key == KEY_K)
		cam->pitch -= 0.1;
	else
		return (false);
	cam->pitch = clampf(cam->pitch, -1.55, 1.55);
	cam->orient_v.x = cosf(cam->pitch) * sinf(cam->yaw);
	cam->orient_v.y = sinf(cam->pitch);
	cam->orient_v.z = cosf(cam->pitch) * cosf(cam->yaw);
	return (true);

}
