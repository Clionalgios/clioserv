#include "logs_internal.h"

static int move_log(const char *logs, const char *saves, const char *name) {
    char save_dir[PATH_MAX], src[PATH_MAX], dst[PATH_MAX];
    if (!logs_build_unique_save_dir(saves, name, save_dir, sizeof save_dir)) return -1;
    if (MKDIR(save_dir) != 0) return -2;
    snprintf(src, sizeof src, "%s/%s", logs, name);
    snprintf(dst, sizeof dst, "%s/%s", save_dir, name);
    return rename(src, dst);
}

#ifdef _WIN32
int logs_archive_existing(const char *logs, const char *saves) {
    char search[MAX_PATH]; WIN32_FIND_DATAA fd;
    snprintf(search, sizeof search, "%s\\*", logs);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    do { if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
         logs_is_log_file(fd.cFileName) && move_log(logs, saves, fd.cFileName)) 
         { FindClose(h); return -2; } } while (FindNextFileA(h, &fd));
    return FindClose(h), 0;
}
#else
int logs_archive_existing(const char *logs, const char *saves) {
    DIR *d = opendir(logs); struct dirent *e;
    if (!d) return -1;
    while ((e = readdir(d))) 
        if (logs_is_log_file(e->d_name) && move_log(logs, saves, e->d_name)) 
            return closedir(d), -2;
    return closedir(d), 0;
}
#endif
