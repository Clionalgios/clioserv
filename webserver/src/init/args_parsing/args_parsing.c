#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "main.h"
#include "args_parsing.h"
#include "init.h"   // doit contenir server_options_t

void set_default_options(server_options_t *opt) {
    opt->config_file = "settings.conf";
    opt->webserver_ip = "127.0.0.1";
    opt->webserver_port = 8181;
    opt->verbose = 0;
    opt->env = "dev";
}

void print_help(const char *prog) {
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("\nOptions:\n");
    printf("  -c, --config <file>     Configuration file\n");
    printf("  -p, --port <port>       Webserver port\n");
    printf("      --ip <addr>         Bind IP\n");
    printf("      --env <env>         Environment (dev|prod)\n");
    printf("  -v, --verbose           Enable verbose mode\n");
    printf("  -h, --help              Show this help\n");
}

int parse_arguments(int argc, char *argv[], app_context_t *ctx) {
    int option_index = 0;

    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"port", required_argument, 0, 'p'},
        {"ip", required_argument, 0, 0},
        {"env", required_argument, 0, 0},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    set_default_options(ctx->options);

    int c;
    while ((c = getopt_long(argc, argv, "c:p:vh", long_options, &option_index)) != -1) {
        switch (c) {

            case 'c':
                ctx->options->config_file = optarg;
                break;

            case 'p':
                ctx->options->webserver_port = atoi(optarg);
                if (ctx->options->webserver_port <= 0 || ctx->options->webserver_port > 65535) {
                    fprintf(stderr, "Invalid port: %s\n", optarg);
                    return -1;
                }
                break;

            case 'v':
                ctx->options->verbose = 1;
                break;

            case 'h':
                print_help(argv[0]);
                exit(0);

            case 0:
                if (strcmp(long_options[option_index].name, "ip") == 0) {
                    ctx->options->webserver_ip = optarg;

                } else if (strcmp(long_options[option_index].name, "env") == 0) {
                    ctx->options->env = optarg;
                }
                break;

            default:
                print_help(argv[0]);
                return -1;
        }
    }

    return 0;
}
