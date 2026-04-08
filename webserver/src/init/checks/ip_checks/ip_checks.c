#include <ip_checks.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

static int is_hex_digit(char c)
{
    return (isdigit((unsigned char)c) ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F'));
}

static int is_valid_ipv4(const char *ip)
{
    int num, dots = 0;
    const char *p = ip;

    while (*p)
    {
        if (!isdigit((unsigned char)*p))
            return 0;

        num = 0;
        if (*p == '0' && isdigit((unsigned char)*(p + 1)))
            return 0; // no leading zeros

        while (*p && isdigit((unsigned char)*p))
        {
            num = num * 10 + (*p - '0');
            if (num > 255)
                return 0;
            p++;
        }

        dots++;
        if (dots > 4)
            return 0;

        if (*p == '.')
            p++;
        else
            break;
    }

    return (dots == 4 && *p == '\0');
}

static int is_valid_ipv6(const char *ip)
{
    int groups = 0;
    int double_colon = 0;
    const char *p = ip;

    if (!ip || *ip == '\0')
        return 0;

    // special case "::"
    if (strcmp(ip, "::") == 0)
        return 1;

    while (*p)
    {
        int count = 0;

        // handle "::"
        if (*p == ':')
        {
            if (*(p + 1) == ':')
            {
                if (double_colon)
                    return 0;
                double_colon = 1;
                p += 2;
                groups++;
                continue;
            }
            else
            {
                return 0; // single ':' unexpected
            }
        }

        // check for IPv4 embedded
        const char *start = p;
        while (*p && *p != ':')
        {
            if (*p == '.')
            {
                // IPv4 part
                if (!is_valid_ipv4(start))
                    return 0;
                groups++; // counts as one group
                return (double_colon ? groups <= 8 : groups == 7);
            }

            if (!is_hex_digit(*p))
                return 0;

            count++;
            if (count > 4)
                return 0;

            p++;
        }

        groups++;
        if (groups > 8)
            return 0;

        if (*p == ':')
            p++;
    }

    if (double_colon)
        return groups <= 8;

    return groups == 8;
}

int is_local_interface_address(const char *addr) {
    if (!addr) return 0;

    // Récupérer la liste des interfaces réseau de la machine
        // Version Linux : utiliser getifaddrs()
        // Version Windows : utiliser GetAdaptersAddresses()

    // Vérifier si l'adresse correspond à une adresse d'interface locale
        // Comparer avec les adresses IPv4 et IPv6 des interfaces

    // Vérifier les adresses de loopback

    return 0; // Retourner 1 si c'est une adresse d'interface locale, sinon 0
}



int is_ip_valid(const char *ip) {
    if (!ip) return 0;

    struct in4_addr ipv4;
    struct in6_addr ipv6;

    // Test IPv4
    if (is_ipv4_syntax_valid(ip)) {
        return 1;
    }

    // Test IPv6
    if (is_ipv6_syntax_valid(ip)) {
        return 1;
    }

    return 0;
}
