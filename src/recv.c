#include "ft_traceroute.h"

static t_msg_data create_message_header(void* message_buffer, int message_len,
                                    void *control_buffer, int control_len)
{
    t_msg_data msg;

    msg = (t_msg_data){0};
    ft_bzero(message_buffer, message_len);
    ft_bzero(control_buffer, control_len);
    msg.msg_iov.iov_base = message_buffer;
    msg.msg_iov.iov_len = message_len;
    msg.msg_hdr.msg_control = control_buffer;
    msg.msg_hdr.msg_controllen = control_len;
    msg.msg_hdr.msg_iov = &msg.msg_iov;
    msg.msg_hdr.msg_iovlen = 1;
    return msg;
}

static void print_trace_info(struct iphdr *ip_hdr, struct icmphdr *icmp_hdr)
{
    if (ip_hdr->saddr != g_traceroute.last_resolved_addr.byte_addr)
    {
        g_traceroute.last_resolved_addr = resolve_ipv4_addr((struct in_addr){ip_hdr->saddr});
        if (g_traceroute.specs.resolve_addr)
            printf("%s (%s)",   g_traceroute.last_resolved_addr.full_addr,
                                g_traceroute.last_resolved_addr.num_addr );
        else
            printf("%s", g_traceroute.last_resolved_addr.num_addr);
    }
    printf("  %.3f ms", usec_time_diff( g_traceroute.send_infos.s_time,
                                        g_traceroute.send_infos.r_time ));
}

static void parse_packet(char *recv_buffer)
{
    struct icmphdr *icmp_hdr;
    struct iphdr *ip_hdr;

    ip_hdr = (struct iphdr *)recv_buffer;
    icmp_hdr = (struct icmphdr *)(recv_buffer + (ip_hdr->ihl << 2));
    if (icmp_hdr->type == ICMP_TIME_EXCEEDED)
    {
        struct iphdr *inner_ip = (struct iphdr *)((void *)(icmp_hdr + 1));
        struct icmphdr *inner_icmp = (struct icmphdr*)((void *)inner_ip + (inner_ip->ihl << 2));
        if(inner_icmp->un.echo.id == my_htons(getpid()))
            print_trace_info(ip_hdr, inner_icmp);

    }
    else if (icmp_hdr->type == ICMP_ECHOREPLY)
    {
        if(icmp_hdr->un.echo.id == my_htons(getpid()))
            print_trace_info(ip_hdr, icmp_hdr);
    }
    else
        printf("* ");
}

void recv_probe(void)
{
    static char                 recv_buffer[IP_MAXPACKET];
    t_msg_data                  re_msg;

    re_msg = create_message_header( recv_buffer,
                                    sizeof(recv_buffer),
                                    NULL, 0 );
    if (recvmsg(g_traceroute.sockfd, &re_msg.msg_hdr, MSG_WAITALL) > 0)
    {
        g_traceroute.send_infos.r_time = get_timeval();
        parse_packet(recv_buffer);
    }
    else
        printf("* ");
}