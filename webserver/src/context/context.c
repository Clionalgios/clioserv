#include <stdlib.h>
#include <string.h>
#include "mongoose.h"
#include "config.h"
#include "runtime_data.h"
#include "context.h"

// /* ======================
//    LIFECYCLE
//    ====================== */

app_context_t *app_context_create(void) {
    app_context_t *app_context = calloc(1, sizeof(app_context_t));
    if (!app_context) return NULL;

    app_context->argc = 0;
    app_context->argv = NULL;

    app_context->config = calloc(1, sizeof(app_config_t));


    app_context->runtime_data = calloc(1, sizeof(runtime_data_t));

    if (!app_context->config || !app_context->runtime_data) {
        free(app_context->config);
        free(app_context->runtime_data);
        free(app_context);
        return NULL;
    }

    app_context->state = APP_STATE_INIT;

    return app_context;
}

void app_context_destroy(app_context_t *app_context) {
    if (!app_context) return;

    free(app_context->runtime_data->webserver_ip);
    free(app_context->runtime_data->webserver_port);
    free(app_context->runtime_data->env);
    free(app_context->runtime_data->banner);
    free(app_context->config);
    free(app_context->runtime_data);

    mg_mgr_free(&app_context->mgr);

    free(app_context);
}

// /* ======================
//    STATE
//    ====================== */

// app_state_t app_context_get_state(app_context_t *ctx) {
//     return ctx->state;
// }

// void app_context_set_state(app_context_t *ctx, app_state_t state) {
//     ctx->state = state;
// }

/*
 * NOTE:
 * - Toutes les fonctions sont NON static (exportées)
 * - Aucun include lourd inutile
 * - Accès direct au contexte
 */

/* =========================
   GETTERS CONFIG
   ========================= */

struct app_config *app_context_get_options(app_context_t *ctx) {
    return ctx ? ctx->config : NULL;
}

const char *app_context_get_webserver_ip(app_context_t *ctx) {
    if (!ctx || !ctx->runtime_data) return NULL;
    return ctx->runtime_data->webserver_ip;
}

const char *app_context_get_webserver_port(app_context_t *ctx) {
    if (!ctx || !ctx->runtime_data) return NULL;
    return ctx->runtime_data->webserver_port;
}

const char *app_context_get_banner(app_context_t *ctx) {
    if (!ctx || !ctx->runtime_data) return NULL;
    return ctx->runtime_data->banner;
}

/* =========================
   MONGOOSE
   ========================= */

struct mg_mgr *app_context_get_mongoose_manager(app_context_t *ctx) {
    return &ctx->mgr;
}

/* =========================
   CONTROL FLAGS
   ========================= */

int app_context_get_should_stop(app_context_t *ctx) {
    if (!ctx) return 1;
    return ctx->should_stop;
}

void app_context_stop(app_context_t *ctx) {
    if (!ctx) return;
    ctx->should_stop = 1;
}

/* =========================
   CONFIG SETTERS
   ========================= */

// void app_context_set_webserver_config_file(app_context_t *ctx, const char *path) {
//     if (!ctx || !ctx->config) return;

//     if (ctx->config->config_file) {
//         free(ctx->config->config_file);
//     }

//     if (path) {
//         ctx->config->config_file = strdup(path);
//     } else {
//         ctx->config->config_file = NULL;
//     }
// }

/* =========================
   EVENTS
   ========================= */



// /* ======================
//    GETTERS
//    ====================== */

app_config_t *app_context_get_config(app_context_t *ctx)
{
    if (!ctx) {
        return NULL;
    }

    return ctx->config;
}

runtime_data_t *app_context_get_runtime(app_context_t *ctx) {
    return ctx ? ctx->runtime_data : NULL;
}

struct mg_mgr *app_context_get_mg_mgr(app_context_t *ctx) {
    return ctx ? &ctx->mgr : NULL;
}

int app_context_get_argc(app_context_t *ctx) {
    return ctx ? ctx->argc : 0;
}

char **app_context_get_argv(app_context_t *ctx) {
    return ctx ? &ctx->argv : NULL;
}


// /* ======================
//    SETTERS
//    ====================== */

void app_context_set_argc(app_context_t *ctx, int argc) {
    if (!ctx) return;
    ctx->argc = argc;
}

void app_context_set_argv(app_context_t *ctx, char **argv) {
    if (!ctx) return;
    ctx->argv = argv;
}

int app_context_set_webserver_ip(app_context_t *ctx, const char *v) {
    return safe_strdup(&ctx->runtime_data->webserver_ip, v);
}

int app_context_set_webserver_port(app_context_t *ctx, const char *v) {
    return safe_strdup(&ctx->runtime_data->webserver_port, v);
}

int app_context_set_env(app_context_t *ctx, const char *v) {
    return safe_strdup(&ctx->runtime_data->env, v);
}



// /* ======================
//    CONTROL
//    ====================== */

app_state_t app_context_get_state(app_context_t *ctx) {
    return ctx ? ctx->state : APP_STATE_ERROR;
}

void app_context_set_state(app_context_t *ctx, app_state_t state) {
    if (ctx) ctx->state = state;
}