#include "init.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #define PATH_SEP '\\'
    #define MKDIR(path) _mkdir(path)
    #define STRCASECMP _stricmp
#else
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>
    #define PATH_SEP '/'
    #define MKDIR(path) mkdir(path, 0755)
    #define STRCASECMP strcasecmp
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif



init_status_t init_check_assets(void) {
    struct stat st;

    if (stat("./website_assets", &st) != 0)
        return INIT_ASSETS_MISSING;

    if (!S_ISDIR(st.st_mode))
        return INIT_ASSETS_NOT_DIR;

    return INIT_OK;
}

init_status_t init_check_config(void) {
    // Hypothétique
    // TODO: vérifier fichier de config
    return INIT_OK;
}
