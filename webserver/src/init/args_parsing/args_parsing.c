#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "context.h"
// #include "ip_checks.h"
#include "args_parsing.h"
#include "init.h"

void print_help(const char *prog) {
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("\nOptions:\n");
    printf("  -c, --config <file>     Configuration file\n");
    printf("  -i, --ip <addr>         Bind address (e.g. 127.0.0.1)\n");
    printf("  -p, --port <port>       Webserver port\n");
    printf("      --env <env>         Environment (dev|prod)\n");
    printf("  -v, --verbose           Enable verbose mode\n");
    printf("  -h, --help              Show this help\n");
}

int parse_arguments(int argc, char *argv[], app_context_t *ctx) {
    int option_index = 0;

    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"ip", required_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {"env", required_argument, 0, 0},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int c;
    while ((c = getopt_long(argc, argv, "c:i:p:vh", long_options, &option_index)) != -1) {
        switch (c) {

            case 'c':
                app_context_set_webserver_config_file(ctx, optarg);
                break;

            case 'i':
                if (!is_ip_syntax_valid(optarg)) {
                    fprintf(stderr, "Invalid IP address: %s\n", optarg);
                    return -1;
                }
                if (!is_ip_valid(optarg)) {
                    fprintf(stderr, "IP address not valid on this machine: %s\n", optarg);
                    return -1;
                }
                app_context_set_webserver_ip(ctx, optarg);
                break;

            case 'p':
                if (atoi(optarg) <= 0 || atoi(optarg) > 65535) {
                    fprintf(stderr, "Invalid port: %s\n", optarg);
                    return -1;
                }
                app_context_set_webserver_port(ctx, optarg);
                break;

            case 'v':
                app_context_set_verbosity(ctx, 1);
                break;

            case 'h':
                print_help(argv[0]);
                exit(0);

            default:
                print_help(argv[0]);
                return -1;
        }
    }

    return 0;
}
