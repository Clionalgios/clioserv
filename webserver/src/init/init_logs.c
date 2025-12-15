#include "init.h"
#include "prompts.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #include <sys/stat.h>
    #define MKDIR(path) _mkdir(path)
    #define STAT _stat
    #define STRCASECMP _stricmp
#else
    #include <sys/stat.h>
    #include <dirent.h>
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0755)
    #define STAT stat
    #define STRCASECMP strcasecmp
#endif

/* -------------------------------------------------------------------------- */
/* Helpers internes                                                            */
/* -------------------------------------------------------------------------- */

static int is_directory(const char *path) {
    struct STAT st;
    if (STAT(path, &st) != 0)
        return 0;
#ifdef _WIN32
    return (st.st_mode & _S_IFDIR) != 0;
#else
    return S_ISDIR(st.st_mode);
#endif
}

static int ensure_directory(const char *path) {
    if (is_directory(path))
        return 1;

    if (MKDIR(path) != 0)
        return 0;

    return is_directory(path);
}

static int is_log_file(const char *name) {
    size_t len = strlen(name);
    if (len < 4)
        return 0;
    return STRCASECMP(name + len - 4, ".log") == 0;
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
static int resolve_binary_dir(const char *argv0,
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
        return 1;
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


/* Génère logs/saves/<logname>-save[-N] */
static int build_unique_save_dir(const char *saves_root,
                                 const char *log_name,
                                 char *out,
                                 size_t out_size) {
    int index = 0;

    for (;;) {
        if (index == 0)
            snprintf(out, out_size, "%s/%s-save", saves_root, log_name);
        else
            snprintf(out, out_size, "%s/%s-save-%d", saves_root, log_name, index);

        if (!is_directory(out))
            return 1;

        index++;
    }
}

/* -------------------------------------------------------------------------- */
/* Fonction principale                                                         */
/* -------------------------------------------------------------------------- */

init_logs_status_t init_logs(const char *argv0) {
    char bin_dir[PATH_MAX];
    char logs_dir[PATH_MAX];
    char saves_dir[PATH_MAX];

    if (resolve_binary_dir(argv0, bin_dir, sizeof bin_dir))
        return INIT_LOGS_PATH_RESOLUTION_FAILED;

    snprintf(logs_dir, sizeof logs_dir, "%s/logs", bin_dir);
    if (!ensure_directory(logs_dir))
        return INIT_LOGS_CREATE_FAILED;

    snprintf(saves_dir, sizeof saves_dir, "%s/saves", logs_dir);
    if (!ensure_directory(saves_dir))
        return INIT_LOGS_CREATE_FAILED;

#ifdef _WIN32
    char search[MAX_PATH];
    snprintf(search, sizeof search, "%s\\*", logs_dir);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return INIT_LOGS_IO_ERROR;

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        if (!is_log_file(fd.cFileName))
            continue;

        char save_dir[PATH_MAX];
        if (!build_unique_save_dir(saves_dir, fd.cFileName,
                                   save_dir, sizeof save_dir))
            return INIT_LOGS_CREATE_FAILED;

        if (MKDIR(save_dir) != 0)
            return INIT_LOGS_CREATE_FAILED;

        char src[PATH_MAX], dst[PATH_MAX];
        snprintf(src, sizeof src, "%s/%s", logs_dir, fd.cFileName);
        snprintf(dst, sizeof dst, "%s/%s", save_dir, fd.cFileName);

        if (rename(src, dst) != 0)
            return INIT_LOGS_IO_ERROR;

    } while (FindNextFileA(h, &fd));

    FindClose(h);

#else
    DIR *dir = opendir(logs_dir);
    if (!dir)
        return INIT_LOGS_IO_ERROR;

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        if (!is_log_file(ent->d_name))
            continue;

        char save_dir[PATH_MAX];
        if (!build_unique_save_dir(saves_dir, ent->d_name,
                                   save_dir, sizeof save_dir))
            return INIT_LOGS_CREATE_FAILED;

        if (MKDIR(save_dir) != 0)
            return INIT_LOGS_CREATE_FAILED;

        char src[PATH_MAX], dst[PATH_MAX];
        snprintf(src, sizeof src, "%s/%s", logs_dir, ent->d_name);
        snprintf(dst, sizeof dst, "%s/%s", save_dir, ent->d_name);

        if (rename(src, dst) != 0) {
            closedir(dir);
            return INIT_LOGS_IO_ERROR;
        }
    }

    closedir(dir);
#endif

    return INIT_LOGS_OK;
}


/* init_logs : 
Voici le pseudocode de la prochaine fonction que je veux faire. Dis-moi si tu as besoin de plus d'infos :
```/* 
- Signature de la fonction : ```int init_logs(const char *argv0);```,
- Si le répertoire ./clioserv/exploitation/logs/ n'existe pas (le binaire se trouve dans exploitation), le créer (le chemin de référence exact doit partir du binaire, donc de ```<dirname(argv[0])>/logs/```, résolution du chemin avec realpath(argv0, ...) si possible fallback dirname(argv0) si échec)
- si le répertoire existe, regarder si il contient un ou plusieurs fichiers .log
- si ```logs``` n'est pas un répertoire mais un fichier, échouer explicitement avec message d'erreur
- ne pas toucher au contenu du reste du répertoire si il existe
- créer un répertoire saves/ dans le répertoire logs/ susmentionné. Pas besoin de tester si il possède un contenu
- si un fichier .log est présent dans le répertoire logs/, créer un répertoire <nom>-save dans le sous-répertoire logs/saves/ reprenant le nom du fichier dont l'extension est .log (prévoir test avec un fichier.log de test). Si il y a plusieurs logs, répéter l'opération pour chaque fichier log (sachant qu'un fichier log correspond à une session du programme). Un répertoire save par fichier. Déplacer le fichier .log précédemment trouvé et utiliser pour la création du nouveau répertoire de sauvegarde ainsi créé dans ce même répertoire. Suffixe strict. Ne pas prendre en compte la casse ou les variantes.
- ordre de traitement de multiples fichiers .log -> arbitraire
- nom précis du répertoire : server.log → server.log-save/
- si le répertoire server.log-save existe déjà, ajouter un suffixe "-1" au nom. Incrémenter pour chaque cas de collision sans limite, premier libre wins. Insensible à la casse. Extension terminale.
- si mkdir() échoue, ou permissions insuffisantes ou fichier .log non accessible → renvoyer une erreur et arrêter le programme (problématiques de permissions ou autre. Dans tous les cas, trop singulier pour maintenir l'exécution du prog)
```*/