#include "ft_traceroute.h"

static void tracerout_usage(void)
{
    printf("Usage: ft_traceroute [OPTION...] HOST\n");
    printf("Print the route packets trace to network host.\n\n");
    printf(" -n, \tdon't resolve hostnames\n");
    printf(" -m, \tset maximal hop count (default: 64)\n");
    printf(" -q, \tsend NUM probe packets per hop (default: 3)\n");
    printf(" -w, \twait NUM seconds for response (default: 3)\n");
    printf("Manadatory or optional arguments to long options are also mandatory or optional\n");
    printf("for any corresponding short options.\n");
    exit(2);
}

void get_traceroute_opt(int argc, char **argv)
{
    char    opt;
    float   time;
    int64_t max_prob;
    int64_t max_hop;
    int64_t datalen;

    while((opt = getopt(argc, argv, "hn" "z:q:N:f:m:N:")) != EOF)
    {
        switch (opt)
        {
            case 'n':
                g_traceroute.probe_info.resolve_addr = false;
                break;
            case 'm':
                max_hop = ft_atoi(optarg);
                if (max_hop < MIN_TTL || max_hop > MAX_TTL)
                    error(2, 0, ERR_INVALID_MAX_TTL, optarg);
                g_traceroute.probe_info.max_ttl = max_hop;
                break;
            case 'q':
                max_prob = ft_atoi(optarg);
                if (max_prob < MIN_PROBE_NBR || max_prob > MAX_PROBE_NBR)
                    error(2, 0, ERR_INVALID_MAX_PROB, MIN_PROBE_NBR, MAX_PROBE_NBR);
                g_traceroute.probe_info.max_prob_sent = max_prob;
                break;
            case 'w':
                time = ft_atoi(optarg);
                if (time < MIN_WAIT_TIME || time > MAX_WAIT_TIME)
                    error(2, 0, ERR_INVALID_WAIT, optarg);
                g_traceroute.probe_info.wait_time = secs_to_timeval(time);
                break;
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
                error(2, 0, "invalid datalen : `%d'", argv[argc - 2]);
            if (datalen > MAX_DATA_LEN)
                datalen = MAX_DATA_LEN;
            g_traceroute.probe_info.packet_len = datalen;
        }
    }
}
