#include "ft_traceroute.h"

static void fill_icmp_packet(char *packet_buffer, uint16_t packet_len)
{
    struct icmphdr     *icmp_hdr;
    struct timeval  *timestamp;

    icmp_hdr = (struct icmphdr *)packet_buffer;
    ft_bzero(icmp_hdr, sizeof(icmp_hdr));
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->un.echo.sequence = my_htons(g_traceroute.send_infos.current_seq);
    icmp_hdr->un.echo.id = my_htons((uint16_t)getpid());
    icmp_hdr->checksum = in_cksum((uint16_t *)packet_buffer, packet_len);
}

void send_probe(void)
{
    static char     packet_buffer[IP_MAXPACKET];
    int8_t          sendto_status;

    fill_icmp_packet(packet_buffer, PACKET_SIZE);
    sendto_status = sendto( g_traceroute.sockfd,
                            packet_buffer,
                            PACKET_SIZE, 0,
                            &g_traceroute.dest.sock_addr,
                            g_traceroute.dest.addr_info.ai_addrlen);
    if (sendto_status > 0)
    {
        g_traceroute.send_infos.s_time = get_timeval();
        g_traceroute.send_infos.packet_sent++;
        g_traceroute.send_infos.current_seq++;
    }
}