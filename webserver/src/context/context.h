#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include <stdint.h>
#include "mongoose.h"

typedef struct app_context app_context_t;
typedef struct server_options server_options_t;
typedef struct variables variables_t;

/* ======================
   STATE MACHINE
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


/* Lifecycle */
app_context_t *app_context_create(void);
void app_context_destroy(app_context_t *ctx);

/* State */
app_state_t app_context_get_state(app_context_t *ctx);
void app_context_set_state(app_context_t *ctx, app_state_t state);

/* Getters */
server_options_t *app_context_get_options(app_context_t *ctx);
variables_t *app_context_get_vars(app_context_t *ctx);
struct mg_mgr *app_context_get_mongoose_manager(app_context_t *ctx);

const char *app_context_get_webserver_config_file(app_context_t *ctx);
const char *app_context_get_webserver_ip(app_context_t *ctx);
const char *app_context_get_webserver_port(app_context_t *ctx);
const char *app_context_get_router_ip(app_context_t *ctx);
const char *app_context_get_router_port(app_context_t *ctx);
const char *app_context_get_verbosity(app_context_t *ctx);
const char *app_context_get_env(app_context_t *ctx);
const char *app_context_get_banner(app_context_t *ctx);
int app_context_get_should_stop(app_context_t *ctx);

/* Setters */
uint8_t app_context_set_webserver_ip(app_context_t *ctx, const char *ip);
uint8_t app_context_set_webserver_port(app_context_t *ctx, const char *port);
uint8_t app_context_set_banner(app_context_t *ctx, const char *banner);
uint8_t app_context_set_webserver_config_file(app_context_t *ctx, const char *config_file);
int app_context_set_verbosity(app_context_t *ctx, int verbose);
int app_context_set_env(app_context_t *ctx, const char *env);
int app_context_set_should_stop(app_context_t *ctx, int should_stop);

typedef struct {
    struct mg_connection *nc;
    struct mg_http_message *hm;
} app_http_event_t;

void app_context_set_http_event(app_context_t *ctx, app_http_event_t *ev);
app_http_event_t *app_context_get_http_event(app_context_t *ctx);

/* Server state */
int app_context_is_running(app_context_t *ctx);
void app_context_stop(app_context_t *ctx);

#endif
