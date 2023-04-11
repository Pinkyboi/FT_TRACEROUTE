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


#define MAX_ADDR_LEN 255
#define IP_HDR_SIZE 20
#define MAX_DATA_LEN IP_MAXPACKET - IP_HDR_SIZE - ICMP_MINLEN

#define DEFAULT_PACKETLEN 60
#define DEFAULT_MAX_HOP 30
#define DEFAULT_MIN_HOP 1
#define DEFAULT_WAIT_TIME 3
#define DEFAULT_MAX_PROB_SENT 3


#define ERR_RANGE_ARG_MSG       "invalid argument: '%s': out of range: %ld <= value <= %ld"
#define PROGNAME                "ft_tracerout"
#define ERR_INVALID_WAIT        "ridiculous waiting time `%d'"
#define ERR_INVALID_MAX_TTL     "invalid max hop value `%d'"
#define ERR_INVALID_MIN_TTL     "invalid min hop value `%d'"
#define ERR_INVALID_MIN_MAX_TTL "invalid min hop value `%d' greater than max hop value `%d'"
#define ERR_INVALID_MAX_PROB    "number of tries should be between %d and %d'"

#define MIN_PROBE_NBR 1
#define MAX_PROBE_NBR 10

#define MIN_TTL 1
#define MAX_TTL MAXTTL

#define MIN_WAIT_TIME 1
#define MAX_WAIT_TIME 60

#define PRINT_LR()              printf("\n")
#define PRINT_TIMEOUT()         printf("  *")
#define GET_RTT()               usec_time_diff(g_traceroute.prob_time.s_time, g_traceroute.prob_time.r_time)
#define PRINT_TTL()             ft_putnbr(g_traceroute.specs.min_ttl) 

#define LAST_ADDRESS            g_traceroute.last_resolved_addr.num_addr
#define LAST_DOMAIN_NAME        g_traceroute.last_resolved_addr.full_addr
#define TARGET_ADDRESS          g_traceroute.resolved_dest.num_addr
#define TARGET_DOMAIN_NAME      g_traceroute.resolved_dest.full_addr

#define SOCKET_FD               g_traceroute.sockfd
#define TIMEOUT                 g_traceroute.specs.timeout
#define PACKET_SIZE             g_traceroute.specs.packet_len

typedef enum bool{

    false,
    true

}       bool;

typedef struct                  s_dest_info
{
    char                        *name;
    struct addrinfo             addr_info;   
    struct sockaddr             sock_addr;
    struct in_addr              bytes_addr;
}                               t_dest_info;

typedef struct                  s_resolved_addr
{
    u_int32_t                   byte_addr;
    char                        full_addr[MAX_ADDR_LEN];
    char                        num_addr[INET_ADDRSTRLEN];
}                               t_resolved_addr;

typedef struct                  s_probe_info
{
    struct timeval              timeout;
    uint8_t                     min_ttl;
    uint8_t                     max_ttl;
    uint8_t                     max_prob_sent;
    uint16_t                    packet_len;
    bool                        resolve_addr;
}                               t_probe_info;

typedef struct                  s_prob_timer
{
    struct timeval              s_time;
    struct timeval              r_time;
}                               t_prob_timer;

typedef struct                  s_tracerout
{
    t_dest_info                 dest;
    t_resolved_addr             last_resolved_addr;
    t_resolved_addr             resolved_dest;
    t_probe_info                specs;
    t_prob_timer                prob_time;
    int                         sockfd;
}                               t_traceroute;

extern t_traceroute              g_traceroute;


void            get_traceroute_opt(int argc, char **argv);
void            error(uint8_t code, int err, char *err_format, ...);

void            get_dest_addr(char *host_name);
t_resolved_addr resolve_ipv4_addr(struct in_addr byte_address);

void           send_probe(uint8_t prob_number);
void           recv_probe(uint8_t prob_number);

//time functions
struct timeval  secs_to_timeval(double secs);
float           usec_time_diff(struct timeval start, struct timeval end);
struct timeval  get_timeval();

uint16_t        my_ntohs(int16_t nshort);
uint16_t        my_htons(int16_t nshort);
uint16_t        in_cksum(uint16_t *buff, uint16_t size);

void            print_traceroute_hdr(void);
void            print_ttl_value(void);
#endif