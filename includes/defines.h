/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:54:19 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/11 15:13:52 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define WIN_WIDTH 800
# define WIN_HEIGHT 600

# ifdef __linux__
#  define KEY_ESC 65307
#  define KEY_W 119
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_H 104
#  define KEY_J 106
#  define KEY_K 107
#  define KEY_L 108
#  define KEY_MINUS 45
#  define KEY_PLUS 61
# endif

# ifdef __APPLE__
#  define KEY_ESC 53
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_H 4
#  define KEY_J 38
#  define KEY_K 40
#  define KEY_L 37
#  define KEY_MINUS 27
#  define KEY_PLUS 24
# endif

/*
 * do we have square?
 */
# define SQUARE 1
# define SPHERE 2
# define PLANE  3
# define CYLINDER 4
# define CONE 5

/* SPHERE DIAMETER ATTRIBUTE INDEX */
# define SPHERE_D_I 0
/* CYLINDER DIAMETER ATTRIBUTE INDEX */
# define CYLINDER_D_I 0
/* CYLINDER HEIGHT ATTRIBUTE INDEX */
# define CYLINDER_H_I 1

/* CONE ANGLE ATTRIBUTE INDEX */
# define CONE_A_I 0
/* CONE HEIGHT ATTRIBUTE INDEX */
# define CONE_H_I 1

#endif
