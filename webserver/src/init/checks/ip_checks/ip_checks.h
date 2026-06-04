#ifndef IP_CHECKS_H
#define IP_CHECKS_H

#include <arpa/inet.h>

// int is_valid_ipv4(const char *ip);
// int is_valid_ipv6(const char *ip);
int is_ip_valid(const char *ip);

struct in4_addr {
	unsigned char   s_addr[4];
};

int is_ip_syntax_valid(const char *ip);

#endif