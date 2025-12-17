#include <sys/stat.h>
#include "fs_helpers.h"
#include "prompts.h"

int is_directory(const char *path) {
    struct STAT st;
    if (STAT(path, &st) != 0)
        return 0;
#ifdef _WIN32
    return (st.st_mode & _S_IFDIR) != 0;
#else
    return S_ISDIR(st.st_mode);
#endif
}

/*
 * Résout le répertoire contenant le binaire en cours d'exécution.
 *
 * Stratégie :
 * - Windows : GetModuleFileNameA(NULL, ...)
 * - POSIX :
 *   1) tentative via realpath(argv0)
 *   2) fallback : extraction du dirname depuis argv0
 *
 * Limites connues :
 * - Sous POSIX, realpath(argv0) peut échouer si argv0 est relatif
 *   ou si le binaire est lancé via $PATH.
 * - Le fallback peut échouer si argv0 ne contient aucun '/'.
 *
 * Paramètres :
 * - argv0     : argv[0] tel que reçu par main()
 * - out       : buffer de sortie
 * - out_size  : taille du buffer
 *
 * Retour :
 * - 1 en cas de succès
 * - 0 en cas d'échec (aucun log fatal ici)
 */
int resolve_current_dir(const char *argv0,
                              char *out,
                              size_t out_size) {
    if (!argv0 || !out || out_size == 0) {
        info_prompt("resolve_binary_dir: invalid arguments");
        return 0;
    }

    info_prompt("resolve_binary_dir: argv0=\"%s\"", argv0);

#ifdef _WIN32
    char buf[MAX_PATH];

    if (GetModuleFileNameA(NULL, buf, sizeof buf) == 0) {
        info_prompt("resolve_binary_dir: GetModuleFileNameA failed");
        return 0;
    }

    info_prompt("resolve_binary_dir: full module path=\"%s\"", buf);

    char *last_sep = strrchr(buf, '\\');
    if (!last_sep) {
        info_prompt("resolve_binary_dir: no directory separator found");
        return 0;
    }

    *last_sep = '\0';
    strncpy(out, buf, out_size - 1);
    out[out_size - 1] = '\0';

    info_prompt("resolve_binary_dir: resolved directory=\"%s\"", out);
    return 1;

#else
    char resolved[PATH_MAX];

    if (realpath(argv0, resolved)) {
        info_prompt("resolve_binary_dir: realpath resolved \"%s\"", resolved);

        char *last_sep = strrchr(resolved, '/');
        if (!last_sep) {
            info_prompt("resolve_binary_dir: no '/' found after realpath");
            return 0;
        }

        *last_sep = '\0';
        strncpy(out, resolved, out_size - 1);
        out[out_size - 1] = '\0';

        info_prompt("resolve_binary_dir: directory from realpath=\"%s\"", out);
        return 0;
    }

    info_prompt("resolve_binary_dir: realpath failed, using fallback");

    const char *slash = strrchr(argv0, '/');
    if (!slash) {
        info_prompt("resolve_binary_dir: fallback failed (no '/' in argv0)");
        return 0;
    }

    size_t len = (size_t)(slash - argv0);
    if (len >= out_size) {
        info_prompt("resolve_binary_dir: output buffer too small");
        return 0;
    }

    strncpy(out, argv0, len);
    out[len] = '\0';

    info_prompt("resolve_binary_dir: directory from fallback=\"%s\"", out);
    return 1;
#endif
}