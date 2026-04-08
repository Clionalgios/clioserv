#include <ctype.h>
#include <string.h>

#ifndef _INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

int is_ip_syntax_valid(const char *addr) {
    if (!addr) return 0;

    if (is_valid_ipv4(addr)) return 1;
    if (is_valid_ipv6(addr)) return 1;

    return 0;
}
