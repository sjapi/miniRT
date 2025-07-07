/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:38:47 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/07 15:59:25 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "utils.h"
#include "renderer.h"
#include "defines.h"

void	render_scene(t_rt *info)
{
	int	i = 0;
	int	j = 0;
	int	amb = get_amb_color(info->scene->amb);

	while (i < WIN_HEIGHT)
	{
		while (j < WIN_WIDTH)
		{
			img_put_pixel_safe(info, i, j, amb);
			j++;
		}
		i++;
	}
}
