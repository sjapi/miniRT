/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:29:57 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/24 14:02:53 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "renderer.h"
#include "defines.h"
#include "utils.h"
#include "mlx.h"

void	calculate_xyz(t_vec3 *x, t_vec3 *y, t_vec3 *z, t_rt *info)
{
	t_vec3	origin;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	origin = (t_vec3){60, 90, 0};
	forward = v_normalize(info->scene->cam->orient_v);
	right = v_normalize(v_cross(forward, (t_vec3){0, 1, 0}));
	up = v_normalize(v_cross(right, forward));
	x->x = origin.x + right.x * 50.0f;
	x->y = origin.y - right.y * 50.0f;
	x->z = 0;
	y->x = origin.x + up.x * 50.0f;
	y->y = origin.y - up.y * 50.0f;
	y->z = 0;
	z->x = origin.x + forward.x * 50.0f;
	z->y = origin.y - forward.y * 50.0f;
	z->z = 0;
}

void	draw_xyz_axis(t_rt *info)
{
	t_vec3	x_end;
	t_vec3	y_end;
	t_vec3	z_end;
	t_vec3	origin;

	origin = (t_vec3){60, 90, 0};
	calculate_xyz(&x_end, &y_end, &z_end, info);
	img_draw_line(info, origin, x_end, 0xff0000);
	img_draw_line(info, origin, y_end, 0x00ff00);
	img_draw_line(info, origin, z_end, 0x0000ff);
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
