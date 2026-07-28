#ifndef APP_TYPES_H
#define APP_TYPES_H

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

/* ======================
   STATES
   ====================== */

typedef enum {
    APP_STATE_INIT = 0,
    APP_STATE_INITIALIZED,
    APP_STATE_RUNNING,
    APP_STATE_STOPPING,
    APP_STATE_STOPPED,
    APP_STATE_ERROR,
    APP_STATE_COUNT
} app_state_t;

#endif
