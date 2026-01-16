#include "logs_internal.h"

int logs_is_log_file(const char *name) {
    size_t len = strlen(name);
    return (len >= 4 && STRCASECMP(name + len - 4, ".log") == 0);
}

int logs_build_unique_save_dir(const char *saves, const char *log, char *out, size_t sz) {
    for (int i = 0; ; i++) {
        snprintf(out, sz, i ? "%s/%s-save-%d" : "%s/%s-save", saves, log, i);
        if (!is_directory(out)) return 1;
    }
}
