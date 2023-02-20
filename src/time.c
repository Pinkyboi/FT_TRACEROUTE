#include "ft_traceroute.h"

struct timeval get_timeval()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv;
}

float usec_time_diff(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec) / 1000.0f;
}

struct timeval secs_to_timeval(double secs)
{
    struct timeval tv;

    tv.tv_sec = (int)secs;
    tv.tv_usec = (int)((secs - tv.tv_sec) * 1000000);
    return (tv);
}
