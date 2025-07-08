/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:46:41 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/08 15:50:48 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

/*
 * origin is position of cam
 */
typedef struct	s_ray
{
	t_point3	origin;
	t_point3	direction;
}	t_ray;

#endif
