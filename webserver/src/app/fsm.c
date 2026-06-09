#include "fsm.h"
#include "actions.h"
#include "context.h"
#include "prompts.h"

typedef struct {
    app_state_t next;
    int (*action)(app_context_t *ctx);
    int valid;
} fsm_transition_t;

static const fsm_transition_t fsm[APP_STATE_COUNT][APP_EVENT_COUNT] = {
    [APP_STATE_INIT] = {
        [APP_EVENT_INIT]  = {APP_STATE_INITIALIZED, action_init, 1},
        [APP_EVENT_FAIL]  = {APP_STATE_ERROR, action_fail, 1}
    },

    [APP_STATE_INITIALIZED] = {
        [APP_EVENT_START] = {APP_STATE_RUNNING, action_start, 1},
        [APP_EVENT_FAIL]  = {APP_STATE_ERROR, action_fail, 1}
    },

    [APP_STATE_RUNNING] = {
        [APP_EVENT_HTTP_REQUEST] = {APP_STATE_RUNNING, handle_http_request, 1},
        [APP_EVENT_TICK]         = {APP_STATE_RUNNING, NULL, 1},
        [APP_EVENT_STOP]         = {APP_STATE_STOPPING, action_stop, 1},
        [APP_EVENT_FAIL]         = {APP_STATE_ERROR, action_fail, 1}
    },

    [APP_STATE_STOPPING] = {
        [APP_EVENT_STOP] = {APP_STATE_STOPPED, NULL, 1}
    },

    [APP_STATE_ERROR] = {
        [APP_EVENT_STOP] = {APP_STATE_STOPPING, action_stop, 1}
    }
};

int fsm_handle_event(app_context_t *ctx, app_event_t event) {
    app_state_t current = app_context_get_state(ctx);
    fsm_transition_t t = fsm[current][event];

    if (!t.valid) {
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
