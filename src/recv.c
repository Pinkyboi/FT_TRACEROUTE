#include "ft_traceroute.h"

static void print_trace_info(struct sockaddr_in *socket_addr)
{
    if (socket_addr->sin_addr.s_addr != g_traceroute.last_resolved_addr.byte_addr)
    {
        g_traceroute.last_resolved_addr = resolve_ipv4_addr((struct in_addr){socket_addr->sin_addr.s_addr});
        if (g_traceroute.specs.resolve_addr)
            printf("  %s (%s)", LAST_ADDRESS, LAST_DOMAIN_NAME);
        else
            printf("  %s", LAST_ADDRESS);
    }
    printf("  %.3f ms", GET_RTT());
}

bool is_packet_mine(char *recv_buffer)
{
    struct iphdr*   ip_hdr;
    struct icmphdr* icmp_hdr;
    struct iphdr*   inner_ip_hdr;
    struct icmphdr* inner_icmp_hdr;

    ip_hdr = (struct iphdr*)recv_buffer;
    icmp_hdr = (struct icmphdr *)(recv_buffer + (ip_hdr->ihl << 2));
    if (icmp_hdr->type == ICMP_TIME_EXCEEDED)
    {
        inner_ip_hdr = (struct iphdr*)((void *)icmp_hdr + sizeof(struct icmphdr));
        inner_icmp_hdr = (struct icmphdr*)((void *)inner_ip_hdr + (inner_ip_hdr->ihl << 2));
        if (inner_icmp_hdr->type == ICMP_ECHO && inner_icmp_hdr->un.echo.id == my_htons(getpid()))
            return true;
    }
    else if (icmp_hdr->type == ICMP_ECHOREPLY && icmp_hdr->un.echo.id == my_htons(getpid()))
        return true;
    return false;
}


void recv_probe(uint8_t prob_number)
{
    static char         recv_buffer[IP_MAXPACKET];
    struct sockaddr     sender_addr;
    struct timeval      current_timeout;
    socklen_t           addrlen;
    fd_set              rfds;
    
    addrlen = sizeof(sender_addr);
    for (int i = 0;  i < prob_number; i++)
    {
        FD_ZERO(&rfds);
        FD_SET(SOCKET_FD, &rfds);
        current_timeout = TIMEOUT;
        if (select(SOCKET_FD + 1, &rfds, NULL, NULL, &current_timeout))
        {
            if (recvfrom(g_traceroute.sockfd, recv_buffer, IP_MAXPACKET, 0, &sender_addr, &addrlen) > 0)
            {
                if (is_packet_mine(recv_buffer))
                {
                    g_traceroute.prob_time.r_time = get_timeval();
                    print_trace_info((struct sockaddr_in *)&sender_addr);
                    continue ;
                }
            }
        }
        PRINT_TIMEOUT();
    }
}