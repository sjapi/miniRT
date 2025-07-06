#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

int	parser(char *file_name)
{
	if (!is_valid_name(file_name))
		return (0);
	return (1);	
}
