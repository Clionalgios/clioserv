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
};

app_context_t *app_context_create(void) {
    app_context_t *ctx = malloc(sizeof(struct app_context));
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
    return ctx;
}

void app_context_destroy(app_context_t *ctx) {
    if (!ctx) return;

    mg_mgr_free(&ctx->mgr);
    free(ctx);
}

int app_context_init(app_context_t *ctx, int argc, char **argv) {
    if (!ctx) return 1;

    if (init(argc, argv, ctx) != 0)
        return 1;

    return 0;
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
        debug_prompt("webserver_ip is NULL");
        exit(1);
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

// ================
// SETTERS
// ================

void app_context_set_webserver_ip(app_context_t *ctx, const char *ip) {
    ctx->options->webserver_ip = ip;
}

void app_context_set_webserver_port(app_context_t *ctx, const char *port) {
    ctx->options->webserver_port = port;
}

void app_context_set_banner(app_context_t *ctx, const char *banner) {
    ctx->vars->banner = banner;
}

void app_context_set_webserver_config_file(app_context_t *ctx, const char *config_file) {
    ctx->options->config_file = config_file;
}

void app_context_set_env(app_context_t *ctx, const char *env) {
    ctx->options->env = env;
}

void app_context_set_verbosity(app_context_t *ctx, int verbose) {
    ctx->options->verbose = verbose;
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
