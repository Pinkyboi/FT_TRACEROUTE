#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <math.h>
#include <unistd.h>
#include <linux/errqueue.h>


#define MAX_C_BUFF_LEN 10240
#define MAX_ADDR_LEN 255
#define IP_HDR_SIZE 20
#define MAX_DATA_LEN IP_MAXPACKET - IP_HDR_SIZE - ICMP_MINLEN

#define DEFAULT_PACKETLEN 40
#define DEFAULT_MAX_HOP 64
#define DEFAULT_WAIT_TIME 3
#define DEFAULT_MAX_PROB_SENT 3


#define ERR_RANGE_ARG_MSG   "invalid argument: '%s': out of range: %ld <= value <= %ld"
#define PROGNAME "ft_tracerout" 

#define PACKET_SIZE ICMP_MINLEN + g_traceroute.probe_info.packet_len

#define ERR_INVALID_WAIT "ridiculous waiting time `%d'"
#define ERR_INVALID_MAX_TTL "invalid hop value `%d'"
#define ERR_INVALID_MAX_PROB "number of tries should be between %d and %d'"

#define MIN_PROBE_NBR 1
#define MAX_PROBE_NBR 10

#define MIN_TTL 1
#define MAX_TTL MAXTTL

#define MIN_WAIT_TIME 1
#define MAX_WAIT_TIME 60



typedef enum bool{

    false,
    true

}       bool;

typedef struct                  s_msg_data
{
    struct msghdr               msg_hdr;
    struct iovec                msg_iov;
}                               t_msg_data;

typedef struct                  s_cmsg_info
{
    struct sock_extended_err    *error_ptr;
    struct cmsghdr              *cmsg;
}                               t_cmsg_info;

typedef struct                  s_dest_info
{
    char                        *name;
    struct addrinfo             addr_info;   
    struct sockaddr             sock_addr;
    struct in_addr              bytes_addr;
}                               t_dest_info;

typedef struct                  s_resolved_addr
{
    char                        full_addr[MAX_ADDR_LEN];
    char                        num_addr[INET_ADDRSTRLEN];
}                               t_resolved_addr;

typedef struct                  s_probe_info
{
    struct timeval              wait_time;
    uint8_t                     min_ttl;
    uint8_t                     max_ttl;
    uint16_t                    packet_len;
    uint32_t                    max_prob_sent;
    bool                        resolve_addr;
    bool                        timestamp;
}                               t_probe_info;

typedef struct                  s_send_infos
{
    uint32_t                    packet_sent;
    uint32_t                    packet_recv;
    uint32_t                    current_seq;
}                               t_send_infos;

typedef struct                  s_tracerout
{
    t_dest_info                 dest;
    t_resolved_addr             last_resolved_addr;
    t_probe_info                probe_info;  
    t_send_infos                send_infos;  
    int                         sockfd;
}                               t_traceroute;

extern t_traceroute              g_traceroute;


void            error(uint8_t code, int err, char *err_format, ...);

void            resolve_ipv4_addr(struct in_addr byte_address);
void            get_dest_addr(char *host_name);

void            get_traceroute_opt(int argc, char **argv);



//time functions
struct timeval  secs_to_timeval(double secs);
float           usec_time_diff(struct timeval start, struct timeval end);
struct timeval  get_timeval();

uint16_t        my_ntohs(int16_t nshort);
uint16_t        my_htons(int16_t nshort);
uint16_t        in_cksum(uint16_t *buff, uint16_t size);

#endif