/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:51:00 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/07 15:53:13 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDNERER_H
# define REDNERER_H

# include "minirt.h"

void	render_scene(t_rt *info);

void	img_put_pixel_safe(t_rt *info, int x, int y, int color);

#endif
