#include "ft_traceroute.h"

t_resolved_addr resolve_ipv4_addr(struct in_addr byte_address)
{
    struct sockaddr_in host_sa;
    t_resolved_addr    resolved_addr;

    resolved_addr = (t_resolved_addr){0};
    resolved_addr.byte_addr = byte_address.s_addr;
    host_sa = (struct sockaddr_in){
        .sin_addr = byte_address,
        .sin_family = AF_INET,
    };

    inet_ntop(host_sa.sin_family, &host_sa.sin_addr,
                    resolved_addr.num_addr,
                    sizeof(resolved_addr.num_addr));
    if (g_traceroute.specs.resolve_addr)
    {
        getnameinfo((struct sockaddr *)&host_sa, sizeof(host_sa),
                        resolved_addr.full_addr,
                        sizeof(resolved_addr.full_addr),
                        NULL, 0, 0);
    }
    return (resolved_addr);
}

void get_dest_addr(char *host_name)
{
    struct addrinfo *server_result;
    struct addrinfo *result_ptr;
    struct addrinfo hints;
    int             status;

    hints = (struct addrinfo){  .ai_family = AF_INET,
                                .ai_socktype = SOCK_RAW,
                                .ai_protocol = IPPROTO_ICMP};
    status = getaddrinfo(host_name, NULL, &hints, &server_result);
    if (status < 0)
        error(2, 0, "%s : %s", host_name, gai_strerror(status));
    for (result_ptr = server_result; result_ptr != NULL; result_ptr = result_ptr->ai_next)
    {
        if ( result_ptr->ai_family == AF_INET &&
                result_ptr->ai_protocol == IPPROTO_ICMP && 
                result_ptr->ai_socktype == SOCK_RAW &&
                result_ptr->ai_addrlen == sizeof(struct sockaddr_in) &&
                result_ptr->ai_addr != NULL )
            break;
    }
    if (result_ptr)
    {
        g_traceroute.dest.addr_info  = *result_ptr;
        g_traceroute.dest.sock_addr  = *result_ptr->ai_addr;
        g_traceroute.dest.bytes_addr = ((struct sockaddr_in *)result_ptr->ai_addr)->sin_addr;
    }
    freeaddrinfo(server_result);
}