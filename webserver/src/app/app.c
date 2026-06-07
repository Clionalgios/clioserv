#include "app.h"
#include "fsm.h"
#include "context.h"
#include "prompts.h"

int app_run(int argc, char **argv) {
    app_context_t *ctx = app_context_create();
    if (!ctx) {
        error_prompt("Failed to create context");
        return 1;
    }

    if (fsm_handle_event(ctx, APP_EVENT_INIT) != 0)
        goto cleanup;

    if (fsm_handle_event(ctx, APP_EVENT_START) != 0)
        goto cleanup;

    if (fsm_handle_event(ctx, APP_EVENT_RUN) != 0)
        goto cleanup;

cleanup:
    fsm_handle_event(ctx, APP_EVENT_STOP);
    app_context_destroy(ctx);
    return 0;
}
