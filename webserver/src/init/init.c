#include <stdio.h>
#include "init.h"

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define STARTUP_BANNER ""

void init(int argc, char *argv[]) {
    printf(CLEAR_SCREEN);

    printf(STARTUP_BANNER);

    // TODO: reset logs files
    init_logs_status_t status = init_logs(argv[0]);

    if (status != INIT_LOGS_OK) {
        error_prompt("logs initialization failed");

        switch (status) {
        case INIT_LOGS_PATH_RESOLUTION_FAILED:
            info_prompt("unable to resolve binary path");
            break;
        case INIT_LOGS_NOT_A_DIRECTORY:
            info_prompt("logs path exists but is not a directory");
            break;
        case INIT_LOGS_CREATE_FAILED:
            info_prompt("failed to create logs directory");
            break;
        case INIT_LOGS_PERMISSION_ERROR:
            info_prompt("permission error while handling logs");
            break;
        case INIT_LOGS_IO_ERROR:
            info_prompt("I/O error while processing existing log files");
            break;
        default:
            break;
        }

        return 1;
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