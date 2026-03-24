#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "init.h"
#include "logs.h"

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define STARTUP_BANNER ""

typedef struct {
    char *config_file;     // default: "settings.conf"

    char *webserver_ip;    // default: "127.0.0.1"
    int webserver_port;    // default: 8181

    int router_mode;       // 0 = standalone, 1 = behind router
    int router_port;       // default: 8080 (dev) / 80 (prod)

    int verbose;           // logs détaillés
    int debug;             // debug mode (plus verbeux + checks)

    int dry_run;           // parse + init sans lancer serveur

    char *log_dir;         // override dossier logs
    int disable_logs;      // désactive logs (utile tests)

    char *env;             // "dev", "prod"

} server_options_t;


// Fonction pour parser les arguments, et les charger dans la structure dédiée

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
    printf("  -c, --config <file>     Configuration file (default: settings.conf)\n");
    printf("  -p, --port <port>      Webserver port (default: 8181)\n");
    printf("      --ip <addr>        Bind IP (default: 127.0.0.1)\n");
    printf("      --env <env>        Environment (dev|prod)\n");
    printf("  -v, --verbose          Enable verbose mode\n");
    printf("  -h, --help             Show this help\n");
}

int parse_arguments(int argc, char *argv[], server_options_t *opt) {
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

    set_default_options(opt);

    int c;
    while ((c = getopt_long(argc, argv, "c:p:vh", long_options, &option_index)) != -1) {
        switch (c) {

            case 'c':
                opt->config_file = optarg;
                break;

            case 'p':
                opt->webserver_port = atoi(optarg);
                if (opt->webserver_port <= 0 || opt->webserver_port > 65535) {
                    fprintf(stderr, "Invalid port: %s\n", optarg);
                    return -1;
                }
                break;

            case 'v':
                opt->verbose = 1;
                break;

            case 'h':
                print_help(argv[0]);
                exit(0);

            case 0:
                if (strcmp(long_options[option_index].name, "ip") == 0) {
                    opt->webserver_ip = optarg;

                } else if (strcmp(long_options[option_index].name, "env") == 0) {
                    opt->env = optarg;

                }
                break;

            default:
                print_help(argv[0]);
                return -1;
        }
    }

    return 0;
}


void init(int argc, char *argv[]) {
    printf(CLEAR_SCREEN);

    printf(STARTUP_BANNER);

    // TODO : parse all arguments
    server_options_t options;

    if (parse_arguments(argc, argv, &options) != 0) {
        fprintf(stderr, "Failed to parse arguments\n");
        exit(1);
    }


    // TODO : load configuration
    // load_configuration(&options.config_file)

    // TODO: reset logs files
    init_logs_status_t status = init_logs(argv[0]);
    if (status != INIT_LOGS_OK) {
        init_logs_failure(status);
    }

    ok_prompt("logs initialized successfully");

    ok_prompt("Initializing Clioserv's webserver...");

    // TODO: load configuration file

    // TODO: parse command-line arguments

    // TODO: externaliser la présente commande dans un futur ensemble init
    // if (!init_check_assets()) {
    //     return;
    // }
}