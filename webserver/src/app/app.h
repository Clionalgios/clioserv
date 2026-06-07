#ifndef APP_H
#define APP_H

#include "context.h"

typedef enum {
    APP_EVENT_INIT = 0,
    APP_EVENT_START,
    APP_EVENT_RUN,
    APP_EVENT_STOP,
    APP_EVENT_FAIL,

    APP_EVENT_HTTP_REQUEST,
    APP_EVENT_TICK,
    // TODO : EVENT_TIMER (timeouts)
    // TODO : EVENT_RELOAD (config)
    // TODO : EVENT_SIGNAL (SIGINT/SIGTERM propre)
    // TODO : EVENT_HEALTH_CHECK

    APP_EVENT_COUNT
} app_event_t;

int app_run(int argc, char **argv);
int app_dispatch(app_context_t *ctx, app_event_t event);

#endif
