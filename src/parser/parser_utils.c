#include <unistd.h>
#include "utils.h"

int	is_valid_name(char *file_name)
{
	int	size;

	size = ft_strlen(file_name);
	if (size < 3)
		return (0);
	if (ft_strcmp(&file_name[size - 3], ".rt") != 0)
		return (0);
	return (1);
}
