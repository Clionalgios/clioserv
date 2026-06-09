#include "app.h"
#include "fsm.h"
#include "context.h"
#include "prompts.h"

int app_step(app_context_t *ctx, app_event_t event) {
    return fsm_handle_event(ctx, event);
}

int app_run(int argc, char **argv) {
    app_context_t *ctx = app_context_create();
    if (!ctx) {
        error_prompt("Failed to create context");
        return 1;
    }

    /* stocker argc/argv dans le context */
    app_context_set_argc(ctx, argc);
    app_context_set_argv(ctx, argv);

    

    if (app_step(ctx, APP_EVENT_INIT) != 0)
        goto cleanup;

    if (app_step(ctx, APP_EVENT_START) != 0)
        goto cleanup;

    server_running(ctx);

cleanup:
    app_step(ctx, APP_EVENT_STOP);
    app_context_destroy(ctx);
    return 0;
}

