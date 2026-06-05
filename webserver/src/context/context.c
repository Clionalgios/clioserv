#include "context.h"
#include "../../common/dependencies/mongoose/mongoose.h"
#include "../../common/src/utils/utils.h"
#include "../../common/src/prompts/prompts.h"
#include "../init/init.h"
#include <stdlib.h>

/* Structure réelle (cachée) */
struct variables {
    char *banner;
};

struct server_options {
    char *config_file;

    char *webserver_ip;
    char *webserver_port;

    char *router_ip;
    char *router_port;

    int verbose;
    int debug;

    int dry_run;

    char *log_dir;
    int disable_logs;

    char *env;
};

struct app_context {
    server_options_t *options;
    variables_t *vars;
    struct mg_mgr mgr;
    uint8_t running;
    volatile sig_atomic_t should_stop;
};

static app_context_t *global_ctx = NULL;

static void safe_free(char **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

static int safe_strdup(char **dst, const char *src) {
    if (!src) return 0;

    char *copy = strdup(src);
    if (!copy) return -1;

    safe_free(dst);
    *dst = copy;
    return 0;
}


void handle_signal(int sig) {
    (void)sig;
    if (global_ctx) {
        global_ctx->should_stop = 1;
    }
}

app_context_t *app_context_create(void) {
    app_context_t *ctx = calloc(1, sizeof(struct app_context));
    if (!ctx) return NULL;

    ctx->options = calloc(1, sizeof(struct server_options));
    ctx->vars = calloc(1, sizeof(struct variables));

    if (!ctx->options || !ctx->vars) {
        free(ctx->options);
        free(ctx->vars);
        free(ctx);
        return NULL;
    }

    ctx->running = 1;
    ctx->should_stop = 0;

    return ctx;
}

static void free_options(server_options_t *opt) {
    if (!opt) return;

    safe_free(&opt->config_file);
    safe_free(&opt->webserver_ip);
    safe_free(&opt->webserver_port);
    safe_free(&opt->router_ip);
    safe_free(&opt->router_port);
    safe_free(&opt->log_dir);
    safe_free(&opt->env);

    free(opt);
}

static void free_vars(variables_t *vars) {
    if (!vars) return;

    safe_free(&vars->banner);

    free(vars);
}

void app_context_destroy(app_context_t *ctx) {
    if (!ctx) return;

    free_options(ctx->options);
    free_vars(ctx->vars);

    mg_mgr_free(&ctx->mgr);

    free(ctx);
}




// ================
// GETTERS
// ================

server_options_t *app_context_get_options(app_context_t *ctx) {
    return ctx->options;
}

variables_t *app_context_get_vars(app_context_t *ctx) {
    return ctx->vars;
}

struct mg_mgr *app_context_get_mongoose_manager(app_context_t *ctx) {
    return &ctx->mgr;
}

const char *app_context_get_webserver_ip(app_context_t *ctx) {
    if (!ctx || !ctx->options || !ctx->options->webserver_ip) {
        return NULL;
    }
    return ctx->options->webserver_ip;
}


const char *app_context_get_webserver_port(app_context_t *ctx) {
    return ctx->options->webserver_port;
}

const char *app_context_get_banner(app_context_t *ctx) {
    return ctx->vars->banner;
}

const char *app_context_get_webserver_config_file(app_context_t *ctx) {
    return ctx->options->config_file;
}

const char *app_context_get_env(app_context_t *ctx) {
    return ctx->options->env;
}

const char *app_context_get_verbosity(app_context_t *ctx) {
    return ctx->options->verbose ? "verbose" : "normal";
}

const char *app_context_get_router_ip(app_context_t *ctx) {
    return ctx->options->router_ip;
}

const char *app_context_get_router_port(app_context_t *ctx) {
    return ctx->options->router_port;
}

int app_context_get_should_stop(app_context_t *ctx) {
    return ctx->should_stop;
}

// ================
// SETTERS
// ================

uint8_t app_context_set_webserver_ip(app_context_t *ctx, const char *ip) {
    return safe_strdup(&ctx->options->webserver_ip, ip);
}


uint8_t app_context_set_webserver_port(app_context_t *ctx, const char *port) {
    return safe_strdup(&ctx->options->webserver_port, port);
}

uint8_t app_context_set_banner(app_context_t *ctx, const char *banner) {
    return safe_strdup(&ctx->vars->banner, banner);
}

uint8_t app_context_set_webserver_config_file(app_context_t *ctx, const char *config_file) {
    return safe_strdup(&ctx->options->config_file, config_file);
}

uint8_t app_context_set_env(app_context_t *ctx, const char *env) {
    return safe_strdup(&ctx->options->env, env);
}

uint8_t app_context_set_verbosity(app_context_t *ctx, int verbose) {
    return safe_strdup(&ctx->options->verbose, verbose ? 1 : 0);
}

uint8_t app_context_set_should_stop(app_context_t *ctx, int should_stop) {
    return safe_strdup(&ctx->should_stop, should_stop ? 1 : 0);
}

// ================
// STATE
// ================

int app_context_is_running(app_context_t *ctx) {
    return ctx->running;
}

void app_context_stop(app_context_t *ctx) {
    ctx->running = 0;
}

struct mg_mgr *app_context_get_mgr(app_context_t *ctx) {
    return &ctx->mgr;
}
