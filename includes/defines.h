/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haaghaja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:54:19 by haaghaja          #+#    #+#             */
/*   Updated: 2025/07/12 11:51:24 by 032zolotarev     ###   ########.fr       */
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

# define SPHERE 2
# define PLANE  3
# define CYLINDER 4
# define CONE 5

# define SPHERE_ATTR_COUNT 2
# define SPHERE_D_I 0 /* SPHERE DIAMETER ATTRIBUTE INDEX */
# define SPHERE_R_I 1 /* SPHERE RADIUS OPTIMIZATION INDEX */

# define CYLINDER_ATTR_COUNT 2
# define CYLINDER_D_I 0 /* CYLINDER DIAMETER ATTRIBUTE INDEX */
# define CYLINDER_H_I 1 /* CYLINDER HEIGHT ATTRIBUTE INDEX */

# define CONE_ATTR_COUNT 8
# define CONE_A_I 0 /* CONE ANGLE ATTRIBUTE INDEX */
# define CONE_H_I 1 /* CONE HEIGHT ATTRIBUTE INDEX */
# define CONE_AP_X_I 2 /* CONE APEX X OPTIMIZATION INDEX */
# define CONE_AP_Y_I 3 /* CONE APEX Y OPTIMIZATION INDEX */
# define CONE_AP_Z_I 4 /* CONE APEX Z OPTIMIZATION INDEX */
# define CONE_AP_Z_I 4 /* CONE APEX Z OPTIMIZATION INDEX */
# define CONE_AR_I 5 /* CONE ANGLE RADIANS OPTIMIZATION INDEX */
# define CONE_TAN2 6 /* CONE TAN(ANGLE_RAD) * TAN(ANGLE_RAD) OPTIMIZATION INDEX */
# define CONE_COS2 7/* CONE COS(ANGLE_RAD) * COS(ANGLE_RAD) OPTIMIZATION INDEX */

#endif
