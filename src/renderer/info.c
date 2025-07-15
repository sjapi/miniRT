/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:29:57 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/15 18:46:12 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "defines.h"
#include "mlx.h"

void	draw_xyz_axis(t_rt *info)
{
	
}

void	draw_info(t_rt *info)
{
	if (info->mode == RENDER_MODE)
		mlx_string_put(info->mlx, info->win, 10, 20, 0xff0000, "mode: render");
	else if (info->mode == OBJECT_MODE)
		mlx_string_put(info->mlx, info->win, 10, 20, 0xff0000, "mode: object");
	else if (info->mode == CAMERA_MODE)
		mlx_string_put(info->mlx, info->win, 10, 20, 0xff0000, "mode: camera");
	else if (info->mode == LIGHT_MODE)
		mlx_string_put(info->mlx, info->win, 10, 20, 0xff0000, "mode: light");
}
