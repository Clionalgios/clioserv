// prompts_time.c
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>   // struct timeval, gettimeofday()
#endif

#define TEXT "clio"

#define RESET  "\x1B[0m"
#define STYLE_BOLD  "\x1B[1m"
#define STYLE_SLIM  "\x1B[2m"
#define STYLE_ITALIC  "\x1B[3m"
#define STYLE_UNDERLINED  "\x1B[4m"
#define STYLE_SLOWBLINK  "\x1B[5m"
#define STYLE_QUICKBLINK  "\x1B[6m"
#define STYLE_REVERSE  "\x1B[7m"
#define STYLE_INVISIBLE  "\x1B[8m"
#define STYLE_CROSSED  "\x1B[9m"

#define BLACK "\x1B[30m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

#define V_BLACK "\x1B[90m"
#define V_RED  "\x1B[91m"
#define V_GREEN  "\x1B[92m"
#define V_YELLOW  "\x1B[93m"
#define V_BLUE  "\x1B[94m"
#define V_MAGENTA  "\x1B[95m"
#define V_CYAN  "\x1B[96m"
#define V_WHITE  "\x1B[97m"

#define BG_BLACK "\x1B[40m"
#define BG_RED  "\x1B[41m"
#define BG_GREEN  "\x1B[42m"
#define BG_YELLOW  "\x1B[43m"
#define BG_BLUE  "\x1B[44m"
#define BG_MAGENTA  "\x1B[45m"
#define BG_CYAN  "\x1B[46m"
#define BG_WHITE  "\x1B[47m"

#define V_BLACK "\x1B[90m"
#define V_RED  "\x1B[91m"
#define V_GREEN  "\x1B[92m"
#define V_YELLOW  "\x1B[93m"
#define V_BLUE  "\x1B[94m"
#define V_MAGENTA  "\x1B[95m"
#define V_CYAN  "\x1B[96m"
#define V_WHITE  "\x1B[97m"

#define BG_V_BLACK "\x1B[100m"
#define BG_V_RED  "\x1B[101m"
#define BG_V_GREEN  "\x1B[102m"
#define BG_V_YELLOW  "\x1B[103m"
#define BG_V_BLUE  "\x1B[104m"
#define BG_V_MAGENTA  "\x1B[105m"
#define BG_V_CYAN  "\x1B[106m"
#define BG_V_WHITE  "\x1B[107m"

static FILE *g_session_log = NULL;

typedef enum {
    PROMPT_OK,
    PROMPT_INFO,
    PROMPT_ERROR
} prompt_level_t;

static const struct {
    const char *label;
    const char *color;
} prompt_meta[] = {
    [PROMPT_OK]    = { "OK",    GREEN  },
    [PROMPT_INFO]  = { "INFO",  YELLOW },
    [PROMPT_ERROR] = { "ERROR", RED    }
};

void prompt_set_logfile(FILE *fp)
{
    g_session_log = fp;
}

void log_current_time(char *buf, size_t buf_size)
{
    if (buf == NULL || buf_size == 0) return;

#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(buf, buf_size, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
             st.wYear, st.wMonth, st.wDay,
             st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0) {
        // En cas d'erreur, on met une valeur vide sécurisée
        buf[0] = '\0';
        return;
    }

    struct tm tm_info;
    localtime_r(&tv.tv_sec, &tm_info);

    snprintf(buf, buf_size,
             "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
             tm_info.tm_year + 1900,
             tm_info.tm_mon + 1,
             tm_info.tm_mday,
             tm_info.tm_hour,
             tm_info.tm_min,
             tm_info.tm_sec,
             tv.tv_usec / 1000);
#endif
}

static void prompt_internal(prompt_level_t level,
                            const char *fmt,
                            va_list args)
{
    char timestamp[32];
    log_current_time(timestamp, sizeof timestamp);

    /* Formatage du message utilisateur */
    char msg[256];
    vsnprintf(msg, sizeof msg, fmt, args);

    /* --- Sortie console (inchangée) --- */
    printf(
        STYLE_SLIM "[%s]" RESET " [%s" STYLE_BOLD "%s" RESET "] - %s\n",
        timestamp,
        prompt_meta[level].color,
        prompt_meta[level].label,
        msg
    );

    /* --- Sortie fichier (texte brut) --- */
    if (g_session_log) {
        fprintf(
            g_session_log,
            "[%s] [%s] %s\n",
            timestamp,
            prompt_meta[level].label,
            msg
        );
        fflush(g_session_log);
    }
}

void ok_prompt(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    prompt_internal(PROMPT_OK, fmt, args);
    va_end(args);
}

void info_prompt(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    prompt_internal(PROMPT_INFO, fmt, args);
    va_end(args);
}

void error_prompt(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    prompt_internal(PROMPT_ERROR, fmt, args);
    va_end(args);
}