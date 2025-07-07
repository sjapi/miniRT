/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 032zolotarev <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 09:45:42 by 032zolotarev      #+#    #+#             */
/*   Updated: 2025/07/07 09:52:10 by 032zolotarev     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdbool.h>

static bool	in_range(float val, float min, float max)
{
	return (val >= min && val <= max);
}

bool	v_normalized(t_point3 p)
{
	return (
		in_norm_range(p.x, -1.0, 1.0),
		&& in_norm_range(p.y, -1.0, 1.0),
		&& in_norm_range(p.z, -1.0, 1.0)
	);
}
