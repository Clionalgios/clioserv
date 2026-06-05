#include "app.h"
#include "init.h"
#include "server.h"
#include "context.h"
#include "prompts.h"

int app_init(app_context_t *ctx, int argc, char **argv) {
    return init(argc, argv, ctx);
}

int app_start(app_context_t *ctx) {
    return server_start(ctx);
}

int app_run_loop(app_context_t *ctx) {
    return server_running(ctx);
}

void app_shutdown(app_context_t *ctx) {
    // Plan d'après ChatGPT :
    /* 3. SHUTDOWN
        fermeture sockets
        flush logs
        libération mémoire
    */
    server_stop(ctx);
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
        goto cleanup;
    }

    if (app_init(ctx, argc, argv) != 0) {
        error_prompt("Initialization failed");
        goto cleanup;
    }

    if (app_start(ctx) != 0) {
        error_prompt("Startup failed");
        goto cleanup;
    }

    exit_code = app_run_loop(ctx);

cleanup:
    app_shutdown(ctx);
    app_context_destroy(ctx);
    return exit_code;
}
