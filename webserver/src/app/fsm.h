#ifndef FSM_H
#define FSM_H

#include "context.h"

/* ======================
   EVENTS
   ====================== */

typedef enum {
    APP_EVENT_INIT = 0,
    APP_EVENT_START,
    APP_EVENT_RUN,
    APP_EVENT_HTTP_REQUEST,
    APP_EVENT_TICK,
    APP_EVENT_STOP,
    APP_EVENT_FAIL,
    APP_EVENT_COUNT
} app_event_t;

int fsm_handle_event(app_context_t *ctx, app_event_t event);

#endif
