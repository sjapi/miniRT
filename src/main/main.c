/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azolotar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:20:31 by azolotar          #+#    #+#             */
/*   Updated: 2025/07/05 19:40:11 by azolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "minirt.h"

static bool	init_info(t_rt *info, char *filename)
{
	(void)info;
	(void)filename;
	// 1 step: parse data here
	// 2 step: init mlx etc
	return (true);
}

int	main(int argc, char **argv)
{
	t_rt	info;

	if (argc != 2)
		return (printf("miniRT: wrong arguments count\n"), 1);
	if (!init_info(&info, argv[1]))
		return (1);
	if (!parser(argv[1]))
		return (printf("miniRT: invalid file\n"), 1);
	printf("hi there\n");
	return (printf("\n"));
}
