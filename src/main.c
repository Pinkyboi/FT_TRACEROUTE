#include "ft_traceroute.h"

t_traceroute g_traceroute = {
    
    .probe_info = {
        .min_ttl = 1,
        .max_ttl = DEFAULT_MAX_HOP,
        .packet_len = DEFAULT_PACKETLEN,
        .wait_time = DEFAULT_WAIT_TIME,
        .max_prob_sent = DEFAULT_MAX_PROB_SENT,
        .resolve_addr = true,
    }

};

int main(int argc, char **argv)
{
    get_traceroute_opt(argc, argv);
    get_dest_addr(g_traceroute.dest.name);
    resolve_ipv4_addr(g_traceroute.dest.bytes_addr);
    if (!strcmp(g_traceroute.dest.name, g_traceroute.last_resolved_addr.num_addr))
        g_traceroute.probe_info.resolve_addr = false;
}