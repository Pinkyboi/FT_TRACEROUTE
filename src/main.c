#include "ft_traceroute.h"

t_traceroute g_traceroute = {
    
    .specs = {
        .min_ttl = MIN_TTL,
        .max_ttl = DEFAULT_MAX_HOP,
        .packet_len = DEFAULT_PACKETLEN,
        .timeout = DEFAULT_WAIT_TIME,
        .max_prob_sent = DEFAULT_MAX_PROB_SENT,
        .resolve_addr = true,
        .timestamp = true
    }

};

void setup_ttl(void)
{
    bool socketfail;

    socketfail = setsockopt( g_traceroute.sockfd,
                            IPPROTO_IP, IP_TTL,
                            (void *)&g_traceroute.specs.min_ttl,
                            sizeof(g_traceroute.specs.min_ttl) );
    if (socketfail)
        error(2, errno, "Internal error");
}

void print_traceroute_hdr()
{
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
        g_traceroute.dest.name,
        g_traceroute.resolved_dest.num_addr,
        g_traceroute.specs.max_ttl,
        g_traceroute.specs.packet_len);
}

void print_ttl_value()
{
    printf("%s%d  ", ((g_traceroute.specs.min_ttl > 9) ? "" : " "), g_traceroute.specs.min_ttl);
}

void traceroute_routine()
{
    print_traceroute_hdr();
    while (g_traceroute.specs.min_ttl < g_traceroute.specs.max_ttl)
    {
        setup_ttl();
        print_ttl_value();
        for (uint8_t i = 0; i < g_traceroute.specs.max_prob_sent; i++)
        {
            fflush(stdout);
            send_probe();
            recv_probe();
        }
        printf("\n");
        if (g_traceroute.last_resolved_addr.byte_addr == g_traceroute.dest.bytes_addr.s_addr)
            break;
        g_traceroute.specs.min_ttl++;
    }
}

void setup_socket(void)
{
    bool socketfail;

    g_traceroute.sockfd = socket( g_traceroute.dest.addr_info.ai_family,
                                SOCK_RAW,
                                IPPROTO_ICMP );
    if (g_traceroute.sockfd < 0)
        error(2, errno, "%s", "socket error");
    socketfail = setsockopt( g_traceroute.sockfd,
                            SOL_SOCKET, SO_RCVTIMEO,
                            (void *)&g_traceroute.specs.timeout,
                            sizeof(g_traceroute.specs.timeout) );
    if (socketfail)
        error(2, 0, "Internal error");
}

int main(int argc, char **argv)
{
    get_traceroute_opt(argc, argv);
    get_dest_addr(g_traceroute.dest.name);
    g_traceroute.resolved_dest = resolve_ipv4_addr(g_traceroute.dest.bytes_addr);
    if (!strcmp(g_traceroute.dest.name, g_traceroute.resolved_dest.num_addr))
        g_traceroute.specs.resolve_addr = false;
    setup_socket();
    traceroute_routine();
}