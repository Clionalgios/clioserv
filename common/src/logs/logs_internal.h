#ifndef LOGS_INTERNAL_H
#define LOGS_INTERNAL_H

#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
    #define STRCASECMP _stricmp
    #define PATH_SEP "\\"
#else
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0755)
    #define STRCASECMP strcasecmp
    #define PATH_SEP "/"
#endif

#include "logs.h"
#include "prompts.h"
#include "fs_helpers.h"

int  logs_is_log_file(const char *name);
int  logs_build_unique_save_dir(const char *saves, const char *log, char *out, size_t sz);
int  logs_archive_existing(const char *logs_dir, const char *saves_dir);
int  logs_create_session(const char *logs_dir, char *out, size_t sz);
void logs_exit_with_status(init_logs_status_t status);

#endif
