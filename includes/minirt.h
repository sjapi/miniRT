/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:16:38 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/05 16:25:49 by haaghaja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>




typedef struct s_obj
{
	int	type;
	float	x;
	float	y;
	float	z;
}	t_obj;

#endif
