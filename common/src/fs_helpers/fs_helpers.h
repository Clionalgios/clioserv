#ifndef INIT_H
#define INIT_H
#endif

#ifdef _WIN32
    #define STAT _stat
#else
    #define STAT stat
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#if !defined(_SIZE_T)
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

int is_directory(const char *path);

int resolve_current_dir(const char *argv0,
                              char *out,
                              size_t out_size);