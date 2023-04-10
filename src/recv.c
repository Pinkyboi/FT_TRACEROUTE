#include "ft_traceroute.h"

static void print_trace_info(struct sockaddr_in *socket_addr)
{
    if (socket_addr->sin_addr.s_addr != g_traceroute.last_resolved_addr.byte_addr)
    {
        g_traceroute.last_resolved_addr = resolve_ipv4_addr((struct in_addr){socket_addr->sin_addr.s_addr});
        if (g_traceroute.specs.resolve_addr)
            printf(" %s (%s)", LAST_ADDRESS, LAST_DOMAIN_NAME);
        else
            printf(" %s", LAST_ADDRESS);
    }
    printf(" %.3f ms", GET_RTT());
}

void recv_probe(uint8_t prob_number)
{
    static char         recv_buffer[IP_MAXPACKET];
    struct sockaddr     sender_addr;
    socklen_t           addrlen;
    fd_set              rfds;
    
    addrlen = sizeof(sender_addr);
    for (int i = 0;  i < prob_number; i++)
    {
        fflush(stdout);
        FD_ZERO(&rfds);
        FD_SET(SOCKET_FD, &rfds);
        TIMEOUT = (struct timeval){.tv_sec = DEFAULT_WAIT_TIME};
        if (select(SOCKET_FD + 1, &rfds, NULL, NULL, &TIMEOUT))
        {
            if (recvfrom(g_traceroute.sockfd, recv_buffer, IP_MAXPACKET, 0, &sender_addr, &addrlen))
            {
                g_traceroute.prob_time.r_time = get_timeval();
                print_trace_info((struct sockaddr_in *)&sender_addr);
                continue ;
            }
        }
        PRINT_TIMEOUT();
    }
}