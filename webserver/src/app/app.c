#include "app.h"
#include "init.h"
#include "server.h"
#include "context.h"
#include "prompts.h"

static int app_set_state(app_context_t *ctx, app_state_t expected, app_state_t next) {
    if (app_context_get_state(ctx) != expected) {
        error_prompt("Invalid state transition");
        app_context_set_state(ctx, APP_STATE_ERROR);
        return -1;
    }

    app_context_set_state(ctx, next);
    return 0;
}

int app_init(app_context_t *ctx, int argc, char **argv) {
    if (app_set_state(ctx, APP_STATE_INIT, APP_STATE_INITIALIZED) != 0)
        return -1;

    if (init(argc, argv, ctx) != 0) {
        app_context_set_state(ctx, APP_STATE_ERROR);
        return -1;
    }

    return 0;
}

int app_start(app_context_t *ctx) {
    if (app_set_state(ctx, APP_STATE_INITIALIZED, APP_STATE_STARTED) != 0)
        return -1;

    if (server_start(ctx) != 0) {
        app_context_set_state(ctx, APP_STATE_ERROR);
        return -1;
    }

    return 0;
}

int app_run_loop(app_context_t *ctx) {
    if (app_set_state(ctx, APP_STATE_STARTED, APP_STATE_RUNNING) != 0)
        return -1;

    int rc = server_running(ctx);

    if (rc != 0) {
        app_context_set_state(ctx, APP_STATE_ERROR);
        return rc;
    }

    return 0;
}


void app_shutdown(app_context_t *ctx) {
    // Plan d'après ChatGPT :
    /* 3. SHUTDOWN
        fermeture sockets
        flush logs
        libération mémoire
    */
   
    if (!ctx) return;

    app_state_t state = app_context_get_state(ctx);

    if (state == APP_STATE_STARTED || state == APP_STATE_RUNNING) {
        app_context_set_state(ctx, APP_STATE_STOPPING);

        server_stop(ctx);

        app_context_set_state(ctx, APP_STATE_STOPPED);
    }
}

int app_run(int argc, char **argv) {
    // Plan
    /* 2. RUN
        boucle principale (serveur HTTP)
        traitement des requêtes
        gestion signaux (flag stop)
    */

    int exit_code = 1;
    app_context_t *ctx = NULL;

    ctx = app_context_create();
    if (!ctx) {
        error_prompt("Failed to create context");
        return 1;
    }

    if (app_init(ctx, argc, argv) != 0)
        goto cleanup;

    if (app_start(ctx) != 0)
        goto cleanup;

    exit_code = app_run_loop(ctx);

cleanup:
    app_shutdown(ctx);
    app_context_destroy(ctx);
    return exit_code;
}