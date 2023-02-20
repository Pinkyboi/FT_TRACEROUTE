#include "ft_traceroute.h"

void error(uint8_t code, int err, char *err_format, ...)
{
    va_list args;

    fprintf(stderr, "%s: ", PROGNAME);
    va_start(args, err_format);
    vfprintf(stderr, err_format, args);
    if (err)
        fprintf(stderr, ": %s", strerror(err));
    va_end(args);
    fprintf(stderr, "\n");
    exit(code);
}
