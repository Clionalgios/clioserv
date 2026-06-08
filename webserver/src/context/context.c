#include "context.h"
#include <stdlib.h>
#include <string.h>

/* ======================
   STRUCTS PRIVÉES
   ====================== */

struct server_options {
    char *config_file;
    char *webserver_ip;
    char *webserver_port;
    char *env;
    int verbose;
};

struct variables {
    char *banner;
};

struct app_context {
    int argc;
    char **argv;
    server_options_t *options;
    variables_t *vars;

    struct mg_mgr mgr;

    uint8_t running;
    app_state_t state;

    volatile sig_atomic_t should_stop;

    app_http_event_t http_event;
};

/* ======================
   UTILS
   ====================== */

static int safe_strdup(char **dst, const char *src) {
    if (!src) return 0;

    char *copy = strdup(src);
    if (!copy) return -1;

    free(*dst);
    *dst = copy;
    return 0;
}

/* ======================
   LIFECYCLE
   ====================== */

app_context_t *app_context_create(void) {
    app_context_t *ctx = calloc(1, sizeof(app_context_t));
    if (!ctx) return NULL;

    ctx->argc = 0;
    ctx->argv = NULL;

    ctx->options = calloc(1, sizeof(server_options_t));
    ctx->vars = calloc(1, sizeof(variables_t));

    if (!ctx->options || !ctx->vars) {
        free(ctx->options);
        free(ctx->vars);
        free(ctx);
        return NULL;
    }

    ctx->running = 1;
    ctx->state = APP_STATE_INIT;

    return ctx;
}

void app_context_destroy(app_context_t *ctx) {
    if (!ctx) return;

    free(ctx->options->config_file);
    free(ctx->options->webserver_ip);
    free(ctx->options->webserver_port);
    free(ctx->options->env);

    free(ctx->vars->banner);

    free(ctx->options);
    free(ctx->vars);

    mg_mgr_free(&ctx->mgr);

    free(ctx);
}

/* ======================
   STATE
   ====================== */

app_state_t app_context_get_state(app_context_t *ctx) {
    return ctx->state;
}

void app_context_set_state(app_context_t *ctx, app_state_t state) {
    ctx->state = state;
}

/* ======================
   HTTP EVENT
   ====================== */

void app_context_set_http_event(app_context_t *ctx, app_http_event_t *ev) {
    if (!ctx || !ev) return;
    ctx->http_event = *ev;
}

app_http_event_t *app_context_get_http_event(app_context_t *ctx) {
    if (!ctx) return NULL;
    return &ctx->http_event;
}

/* ======================
   GETTERS
   ====================== */

server_options_t *app_context_get_options(app_context_t *ctx) {
    return ctx->options;
}

const char *app_context_get_webserver_ip(app_context_t *ctx) {
    return ctx->options->webserver_ip;
}

const char *app_context_get_webserver_port(app_context_t *ctx) {
    return ctx->options->webserver_port;
}

const char *app_context_get_banner(app_context_t *ctx) {
    return ctx->vars->banner;
}

struct mg_mgr *app_context_get_mongoose_manager(app_context_t *ctx) {
    return &ctx->mgr;
}

int app_context_get_should_stop(app_context_t *ctx) {
    return ctx->should_stop;
}

int app_context_get_argc(app_context_t *ctx) {
    return ctx->argc;
}

char **app_context_get_argv(app_context_t *ctx) {
    return ctx->argv;
}


/* ======================
   SETTERS
   ====================== */

void app_context_set_argc(app_context_t *ctx, int argc) {
    if (!ctx) return;
    ctx->argc = argc;
}

void app_context_set_argv(app_context_t *ctx, char **argv) {
    if (!ctx) return;
    ctx->argv = argv;
}

int app_context_set_webserver_config_file(app_context_t *ctx, const char *v) {
    return safe_strdup(&ctx->options->config_file, v);
}

int app_context_set_webserver_ip(app_context_t *ctx, const char *v) {
    return safe_strdup(&ctx->options->webserver_ip, v);
}

int app_context_set_webserver_port(app_context_t *ctx, const char *v) {
    return safe_strdup(&ctx->options->webserver_port, v);
}

int app_context_set_env(app_context_t *ctx, const char *v) {
    return safe_strdup(&ctx->options->env, v);
}

int app_context_set_verbosity(app_context_t *ctx, int v) {
    ctx->options->verbose = v;
    return 0;
}

/* ======================
   CONTROL
   ====================== */

int app_context_is_running(app_context_t *ctx) {
    return ctx->running;
}

void app_context_stop(app_context_t *ctx) {
    ctx->running = 0;
}
