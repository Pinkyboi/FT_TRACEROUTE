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

void read_response(void)
{
    static char                 control_buffer[MAX_C_BUFF_LEN];
    struct icmphdr              icmp_hdr;
    t_msg_data                  err_msg;
    t_cmsg_info                 cmsg_info;

    err_msg = create_message_header(&icmp_hdr, sizeof(icmp_hdr),
        control_buffer, sizeof(control_buffer));
    if (recvmsg(g_traceroute.sockfd, &err_msg.msg_hdr, MSG_ERRQUEUE | MSG_DONTWAIT) > 0)
    {
        cmsg_info = (t_cmsg_info){.cmsg = CMSG_FIRSTHDR(&err_msg.msg_hdr)};
        while (cmsg_info.cmsg)
        {
            if (cmsg_info.cmsg->cmsg_level == IPPROTO_IP &&
                    cmsg_info.cmsg->cmsg_type == IP_RECVERR)
                cmsg_info.error_ptr = (struct sock_extended_err *)CMSG_DATA(cmsg_info.cmsg);
            cmsg_info.cmsg = CMSG_NXTHDR(&err_msg.msg_hdr, cmsg_info.cmsg);
        }
        if (cmsg_info.error_ptr)
        {
            if (cmsg_info.error_ptr->ee_origin == SO_EE_ORIGIN_ICMP)
            {
                if (cmsg_info.error_ptr->ee_type == ICMP_TIME_EXCEEDED)
                    printf("Time exceeded\n");
                else if (cmsg_info.error_ptr->ee_type == ICMP_DEST_UNREACH)
                    printf("Destination unreachable\n");
            }
        }
    }
}