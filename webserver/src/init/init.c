#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "init.h"
#include "logs.h"
#include "main.h"

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define STARTUP_BANNER ""

void init(int argc, char *argv[], app_context_t *ctx) {
    printf(CLEAR_SCREEN);

    printf(STARTUP_BANNER);

    // TODO : parse all arguments

    if (parse_arguments(argc, argv, ctx->options) != 0) {
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