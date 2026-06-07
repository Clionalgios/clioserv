#include "app.h"
#include "init.h"
#include "server.h"
#include "prompts.h"

static int action_http_request(app_context_t *ctx) {
    app_http_event_t *ev = app_context_get_http_event(ctx);

    if (!ev || !ev->nc || !ev->hm)
        return -1;

    router_dispatch(ev->nc, ev->hm, ctx);

    return 0;
}

/* ======================
   DEBUG
   ====================== */

static const char *state_str[] = {
    "INIT",
    "INITIALIZED",
    "STARTED",
    "RUNNING",
    "STOPPING",
    "STOPPED",
    "ERROR"
};

/* ======================
   FSM TYPES
   ====================== */

typedef struct {
    app_state_t next;
    int (*action)(app_context_t *ctx);
} fsm_transition_t;

/* ======================
   ACTIONS
   ====================== */

static int action_init(app_context_t *ctx);
static int action_start(app_context_t *ctx);
static int action_run(app_context_t *ctx);
static int action_stop(app_context_t *ctx);
static int action_fail(app_context_t *ctx);

// ======================
//  FSM TABLE
// ======================

/* FSM : Finite State Machine */
static const fsm_transition_t fsm[APP_STATE_COUNT][APP_EVENT_COUNT] = {
    [APP_STATE_INIT] = {
        [APP_EVENT_INIT]  = {APP_STATE_INITIALIZED, action_init},
        [APP_EVENT_FAIL]  = {APP_STATE_ERROR, action_fail}
    },

    [APP_STATE_INITIALIZED] = {
        [APP_EVENT_START] = {APP_STATE_STARTED, action_start},
        [APP_EVENT_FAIL]  = {APP_STATE_ERROR, action_fail}
    },

    [APP_STATE_STARTED] = {
        [APP_EVENT_RUN]  = {APP_STATE_RUNNING, action_run},
        [APP_EVENT_STOP] = {APP_STATE_STOPPING, action_stop},
        [APP_EVENT_FAIL] = {APP_STATE_ERROR, action_fail}
    },

    [APP_STATE_RUNNING] = {
        [APP_EVENT_HTTP_REQUEST] = {APP_STATE_RUNNING, action_http_request},
        [APP_EVENT_TICK] = {APP_STATE_RUNNING, NULL},
        [APP_EVENT_STOP] = {APP_STATE_STOPPING, action_stop},
        [APP_EVENT_FAIL] = {APP_STATE_ERROR, action_fail}
    },


    [APP_STATE_STOPPING] = {
        [APP_EVENT_STOP] = {APP_STATE_STOPPED, NULL}
    },

    [APP_STATE_ERROR] = {
        [APP_EVENT_STOP] = {APP_STATE_STOPPING, action_stop}
    }
};

/* ======================
   DISPATCHER
   ====================== */

static int app_dispatch(app_context_t *ctx, app_event_t event) {
    app_state_t current = app_context_get_state(ctx);
    fsm_transition_t t = fsm[current][event];

    if (t.next == 0 && t.action == NULL) {
        error_prompt("Invalid event for current state");
        return -1;
    }

    if (t.action && t.action(ctx) != 0) {
        app_context_set_state(ctx, APP_STATE_ERROR);
        return -1;
    }

    app_context_set_state(ctx, t.next);

    return 0;
}

/* ======================
   ACTIONS IMPL
   ====================== */

static int action_init(app_context_t *ctx) {
    return init(0, NULL, ctx); // TODO: passer argc/argv proprement
}

static int action_start(app_context_t *ctx) {
    return server_start(ctx);
}

static int action_run(app_context_t *ctx) {
    return server_running(ctx);
}

static int action_stop(app_context_t *ctx) {
    server_stop(ctx);
    return 0;
}

static int action_fail(app_context_t *ctx) {
    return -1;
}

/* ======================
   ENTRYPOINT
   ====================== */

int app_run(int argc, char **argv) {
    app_context_t *ctx = app_context_create();
    if (!ctx) {
        error_prompt("Failed to create context");
        return 1;
    }

    if (app_dispatch(ctx, APP_EVENT_INIT) != 0)
        goto cleanup;

    if (app_dispatch(ctx, APP_EVENT_START) != 0)
        goto cleanup;

    if (app_dispatch(ctx, APP_EVENT_RUN) != 0)
        goto cleanup;

cleanup:
    app_dispatch(ctx, APP_EVENT_STOP);
    app_context_destroy(ctx);
    return 0;
}
