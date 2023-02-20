#include "ft_traceroute.h"

t_tracerout g_tracerout = {
    .probe_info = {
        .min_ttl = 1,
        .max_ttl = DEFAULT_MAX_TTL,
        .packet_len = DEFAULT_PACKETLEN,
        .wait_time = DEFAULT_WAIT_TIME,
        .max_prob_sent = DEFAULT_MAX_PROB_SENT,
        .resolve_addr = true,
    },

};

int main(int argc, char **argv)
{
    get_ping_opt(argc, argv);
    get_dest_addr(g_tracerout.dest.name);
    resolve_ipv4_addr(g_tracerout.dest.bytes_addr);
    if (!strcmp(g_tracerout.dest.name, g_tracerout.last_resolved_addr.num_addr))
        g_tracerout.probe_info.resolve_addr = false;
}