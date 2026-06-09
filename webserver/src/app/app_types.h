#ifndef APP_TYPES_H
#define APP_TYPES_H

struct mg_connection;
struct mg_http_message;

typedef struct app_http_event {
    struct mg_connection *nc;
    struct mg_http_message *hm;
} app_http_event_t;

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
