#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "init.h"
#include "logs.h"
#include "main.h"

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define STARTUP_BANNER ""

void set_default_options(app_context_t *ctx) {
    if (!ctx) {
        fprintf(stderr, "ctx is NULL\n");
        exit(1);
    }

    ctx->options = malloc(sizeof(*(ctx->options)));
    if (!ctx->options) {
        perror("malloc");
        exit(1);
    }

    ctx->options->config_file = "settings.conf";
    ctx->options->webserver_ip = "127.0.0.1";
    ctx->options->webserver_port = "8181";
    ctx->options->verbose = 0;
    ctx->options->env = "dev";
}

void init(int argc, char *argv[], app_context_t *ctx) {
    printf(CLEAR_SCREEN);

    printf(STARTUP_BANNER);

    // TODO : parse all arguments

    set_default_options(ctx);

    printf("Default values set: IP=%s, Port=%s, Env=%s\n",
           ctx->options->webserver_ip ? ctx->options->webserver_ip : "(null)",
           ctx->options->webserver_port ? ctx->options->webserver_port : "(null)",
           ctx->options->env ? ctx->options->env : "(null)");


    if (parse_arguments(argc, argv, ctx) != 0) {
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