#include "tools_cpp.hpp"

size_t	ft_time_seconds(void)
{
	struct timeval	tv;
	size_t			time_in_seconds;

	gettimeofday(&tv, NULL);
	time_in_seconds = (tv.tv_sec) + (tv.tv_usec) / 1000000;
	return (time_in_seconds);
}
