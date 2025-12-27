#ifndef LOGS_H
#define LOGS_H

#if !defined(_SIZE_T)
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;

#endif

typedef enum {
    INIT_LOGS_OK = 0,
    INIT_LOGS_PATH_RESOLUTION_FAILED,
    INIT_LOGS_NOT_A_DIRECTORY,
    INIT_LOGS_CREATE_FAILED,
    INIT_LOGS_PERMISSION_ERROR,
    INIT_LOGS_IO_ERROR
} init_logs_status_t;


init_logs_status_t init_logs(const char *argv0);

// Logs / affichage
void init_log_status(init_logs_status_t status);
int create_session_logfile(const char *logs_dir,
                           char *out_path,
                           size_t out_size);

#endif /* INIT_H */
