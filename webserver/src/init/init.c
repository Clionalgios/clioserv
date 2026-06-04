#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "init.h"
#include "logs.h"
#include "context.h"

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define STARTUP_BANNER ""

void set_default_options(app_context_t *ctx) {
    if (!ctx) {
        fprintf(stderr, "ctx is NULL\n");
        exit(1);
    }

    app_context_set_webserver_config_file(ctx, "./config/webserver.conf");
    app_context_set_webserver_ip(ctx, "127.0.0.1");

    app_context_set_webserver_port(ctx, "8181");
    app_context_set_verbosity(ctx, 0);
    app_context_set_env(ctx, "dev");
}

int init(int argc, char *argv[], app_context_t *ctx) {
    printf(CLEAR_SCREEN);

    printf(STARTUP_BANNER);

    set_default_options(ctx);

    // TODO : load configuration
    // load_configuration(&options.config_file)

    if (parse_arguments(argc, argv, ctx) != 0) {
        fprintf(stderr, "Failed to parse arguments\n");
        exit(1);
    }

    // TODO: reset logs files
    init_logs_status_t status = init_logs(argv[0]);
    if (status != INIT_LOGS_OK) {
        init_logs_failure(status);
    }

    ok_prompt("Logs initialized successfully");

    ok_prompt("Initializing Clioserv's webserver...");

    // TODO: load configuration file

    // TODO: parse command-line arguments

    // TODO: externaliser la présente commande dans un futur ensemble init
    // if (!init_check_assets()) {
    //     return;
    // }
}