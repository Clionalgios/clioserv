#include <stdio.h>
#include "init.h"
#include "logs.h"

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define STARTUP_BANNER ""

void init(int argc, char *argv[]) {
    printf(CLEAR_SCREEN);

    printf(STARTUP_BANNER);


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