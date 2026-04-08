#ifndef _IN_ADDR_DECLARED
#define _IN_ADDR_DECLARED

int is_valid_ipv4(const char *ip);
int is_valid_ipv6(const char *ip);

struct in4_addr {
	unsigned char   s_addr[4];
};

struct in6_addr {
    unsigned char   s6_addr[16];
};

#endif