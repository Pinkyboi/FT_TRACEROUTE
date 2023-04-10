#include "ft_traceroute.h"

static void tracerout_usage(void)
{
    printf("Usage: ft_traceroute [OPTION...] HOST [PACKET_LEN]\n");
    printf("Print the route packets trace to network host.\n\n");
    printf(" -n, \tdon't resolve hostnames\n");
    printf(" -m, \tset maximal hop count (default: %d)\n", DEFAULT_MAX_HOP);
    printf(" -M, \tset minimal hop count (default: %d)\n", DEFAULT_MIN_HOP);
    printf(" -q, \tsend NUM probe packets per hop (default: %d)\n", DEFAULT_MAX_PROB_SENT);
    printf(" -w, \twait NUM seconds for response (default: %d)\n", DEFAULT_WAIT_TIME);
    printf("\nArguments:\n");
    printf("  HOST\t\tThe host to traceroute to\n");
    printf("  PACKET_LEN\tThe full packet length (default is the length of an IP\n");
    printf("\t\theader plus 40). Can be ignored or increased to a minimal\n");
    printf("\t\tallowed value\n");
    exit(2);
}

void get_traceroute_opt(int argc, char **argv)
{
    char    opt;
    float   time;
    int64_t max_prob;
    int64_t max_hop;
    int64_t min_hop;
    int64_t datalen;

    while((opt = getopt(argc, argv, "hn" "w:q:m:M:")) != EOF)
    {
        switch (opt)
        {
            case 'n':
                g_traceroute.specs.resolve_addr = false;
                break;
            case 'm':
                max_hop = ft_atoi(optarg);
                if (max_hop < MIN_TTL || max_hop > MAX_TTL)
                    error(2, 0, ERR_INVALID_MAX_TTL, optarg);
                g_traceroute.specs.max_ttl = max_hop;
                break;
            case 'M':
                min_hop = ft_atoi(optarg);
                if (min_hop < MIN_TTL || min_hop > MAX_TTL)
                    error(2, 0, ERR_INVALID_MIN_TTL, optarg);
                g_traceroute.specs.min_ttl = min_hop;
                break;
            case 'q':
                max_prob = ft_atoi(optarg);
                if (max_prob < MIN_PROBE_NBR || max_prob > MAX_PROBE_NBR)
                    error(2, 0, ERR_INVALID_MAX_PROB, MIN_PROBE_NBR, MAX_PROBE_NBR);
                g_traceroute.specs.max_prob_sent = max_prob;
                break;
            case 'w':
                time = ft_atoi(optarg);
                if (time < MIN_WAIT_TIME || time > MAX_WAIT_TIME)
                    error(2, 0, ERR_INVALID_WAIT, optarg);
                g_traceroute.specs.timeout = secs_to_timeval(time);
                break;
            default:
                tracerout_usage();
            continue;
        }
    }
    if (g_traceroute.specs.max_ttl < g_traceroute.specs.min_ttl)
    {
        error(2, 0, ERR_INVALID_MIN_MAX_TTL, g_traceroute.specs.min_ttl, g_traceroute.specs.max_ttl);
    }
	argc -= optind;
	argv += optind;
    if (!argc)
        tracerout_usage();
    else
    {
        if (argc > 1)
        {
            datalen = ft_atoi(argv[argc - 1]);
            if (datalen < 0)
                error(2, 0, "invalid datalen : `%d'", argv[argc - 1]);
            if (datalen > MAX_DATA_LEN)
                datalen = MAX_DATA_LEN;
            g_traceroute.specs.packet_len = datalen;
            g_traceroute.dest.name = argv[argc - 2];
        }
        else
            g_traceroute.dest.name = argv[argc - 1];
    }
}
