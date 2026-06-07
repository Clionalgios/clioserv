#ifndef APP_H
#define APP_H

#include <stddef.h>

/* ======================
   FORWARD DECLARATIONS
   ====================== */

typedef struct app_context app_context_t;
typedef struct app_http_event app_http_event_t;

/* ======================
   STATES
   ====================== */

typedef enum {
    APP_STATE_INIT = 0,
    APP_STATE_INITIALIZED,
    APP_STATE_STARTED,
    APP_STATE_RUNNING,
    APP_STATE_STOPPING,
    APP_STATE_STOPPED,
    APP_STATE_ERROR,
    APP_STATE_COUNT
} app_state_t;

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
   HTTP EVENT
   ====================== */

struct app_http_event {
    void *nc;  // mg_connection*
    void *hm;  // mg_http_message*
};

/* ======================
   PUBLIC API
   ====================== */

int app_run(int argc, char **argv);

#endif
