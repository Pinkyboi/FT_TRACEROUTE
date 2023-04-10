#include "ft_traceroute.h"

static void fill_icmp_packet(char *packet_buffer, uint16_t packet_len)
{
    struct icmphdr     *icmp_hdr;

    icmp_hdr = (struct icmphdr *)packet_buffer;
    ft_bzero(icmp_hdr, sizeof(icmp_hdr));
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->un.echo.id = my_htons((uint16_t)getpid());
    icmp_hdr->checksum = in_cksum((uint16_t *)packet_buffer, packet_len);
}

void send_probe(uint8_t prob_number)
{
    static char     packet_buffer[IP_MAXPACKET];

    for (int i = 0; i < prob_number; i++)
    {
        ft_bzero(packet_buffer, IP_MAXPACKET);
        fill_icmp_packet(packet_buffer, PACKET_SIZE);
        sendto( g_traceroute.sockfd,
                packet_buffer,
                PACKET_SIZE, 0,
                &g_traceroute.dest.sock_addr,
                g_traceroute.dest.addr_info.ai_addrlen);
    }
    g_traceroute.prob_time.s_time = get_timeval();
}