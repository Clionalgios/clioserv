#include "logs_internal.h"

static int get_local_time(struct tm *t) {
    time_t now = time(NULL);
    if (now == (time_t)-1) return -1;
#ifdef _WIN32
    return localtime_s(t, &now) == 0 ? 0 : -1;
#else
    return localtime_r(&now, t) ? 0 : -1;
#endif
}

static int format_filename(struct tm *t, char *buf, size_t sz) {
    int n = snprintf(buf, sz, "%04d-%02d-%02d-%02d%02d%02d.log",
        t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return (n > 0 && (size_t)n < sz) ? 0 : -1;
}

static int create_empty_file(const char *path) {
#ifdef _WIN32
    FILE *fp = fopen(path, "w");
#else
    FILE *fp = fopen(path, "wx");
#endif
    return fp ? (fclose(fp), 0) : -1;
}

int logs_create_session(const char *dir, char *out, size_t sz) {
    struct tm t; char fname[32];
    if (get_local_time(&t) || format_filename(&t, fname, sizeof fname)) return -1;
    if (snprintf(out, sz, "%s%s%s", dir, PATH_SEP, fname) <= 0) return -2;
    return create_empty_file(out);
}
