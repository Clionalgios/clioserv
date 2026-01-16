#include "logs_internal.h"

static int setup_directories(const char *argv0, char *logs, char *saves, size_t sz) {
    char bin[PATH_MAX];
    if (resolve_current_dir(argv0, bin, sizeof bin)) return INIT_LOGS_PATH_RESOLUTION_FAILED;
    snprintf(logs, sz, "%s/logs", bin); snprintf(saves, sz, "%s/saves", logs);
    return (!ensure_directory(logs) || !ensure_directory(saves)) ? INIT_LOGS_CREATE_FAILED : 0;
}

static int open_session_log(const char *logs) {
    char path[PATH_MAX]; FILE *fp;
    if (logs_create_session(logs, path, sizeof path)) return -1;
    if (!(fp = fopen(path, "a"))) return -2;
    return prompt_set_logfile(fp), ok_prompt("session log: %s", path), 0;
}

init_logs_status_t init_logs(const char *argv0) {
    char logs[PATH_MAX], saves[PATH_MAX]; int rc;
    if ((rc = setup_directories(argv0, logs, saves, sizeof logs))) return rc;
    if (logs_archive_existing(logs, saves)) return INIT_LOGS_IO_ERROR;
    return open_session_log(logs) ? INIT_LOGS_CREATE_FAILED : INIT_LOGS_OK;
}

int init_logs_failure(init_logs_status_t s) { return logs_exit_with_status(s), 0; }
