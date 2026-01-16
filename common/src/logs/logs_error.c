#include "logs_internal.h"

static const int exit_codes[] = {0, 1, 2, 3, 4, 5};

static const char *msgs[] = {
    NULL, "unable to resolve binary path", "logs path exists but is not a directory",
    "failed to create logs directory", "permission error", "I/O error"
};

void logs_exit_with_status(init_logs_status_t s) {
    error_prompt("logs initialization failed");
    if (s > 0 && s <= 5) error_prompt(msgs[s]);
    exit(exit_codes[s <= 5 ? s : 0]);
}
