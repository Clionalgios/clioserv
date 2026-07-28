#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "init.h"
#include "logs.h"
#include "context.h"
#include "config.h"

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define STARTUP_BANNER ""

int set_default_options(app_context_t *ctx)
{
    if (!ctx || !ctx->config) {
        fprintf(stderr, "Invalid context or configuration\n");
        return -1;
    }

    if (app_config_set_file(ctx->config,
                            "./config/webserver.conf") != 0) {
        return -1;
    }

    if (app_config_set_verbosity(ctx->config, 0) != 0) {
        return -1;
    }

    if (app_context_set_webserver_ip(ctx, "127.0.0.1") != 0) {
        return -1;
    }

    if (app_context_set_webserver_port(ctx, "8181") != 0) {
        return -1;
    }

    if (app_context_set_env(ctx, "dev") != 0) {
        return -1;
    }

    return 0;
}

int init(int argc, char *argv[], app_context_t *ctx) {
    // Plan d'après grand-mestre ChatGPT :
    /* 1. INIT
        parsing args
        chargement config
        initialisation modules
        ouverture ressources (sockets, fichiers…)
    */

    // global_ctx = ctx;
    // signal(SIGINT, handle_signal);
    // signal(SIGTERM, handle_signal);

    warning_prompt("Signal handlers setup is not implemented, the application may not respond correctly to termination signals");
    // TODO integrate setup_signal_handlers(ctx); (SIGINT, SIGTERM, etc.)


    printf(CLEAR_SCREEN);
    printf(STARTUP_BANNER);

    if (set_default_options(ctx) != 0) {
        fprintf(stderr, "Failed to set default options\n");
        return -1;
    }

    // TODO : load configuration
    // load_configuration(&options.config_file)

    if (parse_arguments(argc, argv, ctx) != 0) {
        fprintf(stderr, "Failed to parse arguments\n");
        exit(1);
    } else {
        
    }

    // TODO: reset logs files
    init_logs_status_t status = init_logs(ctx->argv[0]);
    if (status != INIT_LOGS_OK) {
        init_logs_failure(status);
    } else {
        ok_prompt("Logs initialized successfully");
    }

    ok_prompt("Initializing Clioserv's webserver...");

    mg_mgr_init(app_context_get_mongoose_manager(ctx));

    warning_prompt("Loading configuration from file is not implemented, using default configuration and command-line arguments only");
    // TODO: load configuration file

    warning_prompt("Command-line arguments parsing is not fully implemented, some options are not yet supported");
    // TODO: parse command-line arguments

    warning_prompt("Asset initialization is not fully implemented");
    // TODO: externaliser la présente commande dans un futur ensemble init



    // if (!init_check_assets()) {
    //     return;
    // }

    ok_prompt("Initialization completed successfully");
}