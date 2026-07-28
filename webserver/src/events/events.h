#ifndef EVENTS_H
#define EVENTS_H

#include "mongoose.h"
#include "app_types.h"

// typedef struct {
//     struct mg_connection *conn;
//     struct mg_http_message *req;
// } http_event_t;

struct mg_connection;
struct mg_http_message;

typedef struct app_http_event {
    struct mg_connection *nc;
    struct mg_http_message *hm;
} app_http_event_t;

typedef struct {
    app_event_t type;

    union {
        app_http_event_t http;

        /* futur */
        // tick_event_t tick;
        // signal_event_t signal;

    } data;

} app_event_data_t;

#endif
