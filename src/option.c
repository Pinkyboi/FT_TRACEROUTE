#include "ft_traceroute.h"

static int64_t strtol_or_err(char *arg, char *err_msg, long min_value, long max_value)
{
    long num_arg;
    char *end_ptr = NULL;

    errno = 0;
    if (arg == NULL || *arg == '\0')
        error(2, 0, err_msg);
    num_arg = strtol(arg, &end_ptr, 10);
    if (errno || arg == end_ptr || (end_ptr && *end_ptr))
        error(2, errno, err_msg, arg);
    if (num_arg < min_value || num_arg > max_value)
        error(2, 0, ERR_RANGE_ARG_MSG, arg, min_value, max_value);
    return (num_arg);
}

static void tracerout_usage(void)
{
    printf("Usage:\n");
    printf("  traceroute [ -n ] [ -f first_ttl ] [ -i device ] [ -m max_ttl ]");
    printf(" [ -N squeries ] [ -q nqueries ] [ -z sendwait ] host [ packetlen ]\n");
    printf("Options:\n");
    printf(" -f first_ttl");
    printf("\t\tStart from the first_ttl hop (instead from 1)\n");
    printf(" -m max_ttl");
    printf("\t\tSet the max number of hops (max TTL to be\n");
    printf("\t\t\treached). Default is 30\n");
    printf(" -N squeries");
    printf("\t\tSet the number of probes to be tried\n");
    printf("\t\t\tsimultaneously (default is 16)\n");
    printf(" -n");
    printf("\t\t\tDo not resolve IP addresses to their domain names\n");
    printf(" -q nqueries");
    printf("\t\tSet the number of probes per each hop. Default is 3\n");
    printf(" --help");
    printf("\t\t\tRead this help and exit\n");
    printf("Arguments:\n");
    printf("+\thost");
    printf("\t\tThe host to traceroute to\n");
    printf("\tpacketlen");
    printf("\tThe full packet length (default is the length of an IP\n");
    printf("\t\t\theader plus 40). Can be ignored or increased to a minimal\n");
    printf("\t\t\tallowed value\n");
    exit(2);
}

void get_traceroute_opt(int argc, char **argv)
{
    char    opt;
    float   secs;
    int64_t datalen;

    while((opt = getopt(argc, argv, "hn" "z:q:N:f:m:N:")) != EOF)
    {
        switch (opt)
        {
            case 'n':
                g_traceroute.probe_info.resolve_addr = false;
                break;
            /// add treatment for diffrent options
            default:
                tracerout_usage();
            continue;
        }
    }
	argc -= optind;
	argv += optind;
    if (!argc)
        tracerout_usage();
    else
    {
        g_traceroute.dest.name = argv[argc - 1];
        if (argc > 1)
        {
            datalen = ft_atoi(argv[argc - 2]);
            if (datalen < 0)
                error(2, 0, "invalid datalen : `%d'", argv[argc - 2])
            if (datalen > MAX_DATA_LEN)
                datalen = MAX_DATA_LEN;
            g_traceroute.probe_info.packet_len = datalen;
        }
    }
}
