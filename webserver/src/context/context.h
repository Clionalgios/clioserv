#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>
#include <signal.h>
#include "mongoose.h"
#include "app_types.h"

typedef struct app_context app_context_t;
typedef struct server_options server_options_t;
typedef struct variables variables_t;

/* ======================
   LIFECYCLE
   ====================== */

app_context_t *app_context_create(void);
void app_context_destroy(app_context_t *ctx);

/* ======================
   STATE
   ====================== */

app_state_t app_context_get_state(app_context_t *ctx);
void app_context_set_state(app_context_t *ctx, app_state_t state);

/* ======================
   HTTP EVENT
   ====================== */

void app_context_set_http_event(app_context_t *ctx, app_http_event_t *ev);
app_http_event_t *app_context_get_http_event(app_context_t *ctx);

/* ======================
   OPTIONS (GET)
   ====================== */

server_options_t *app_context_get_options(app_context_t *ctx);

const char *app_context_get_webserver_ip(app_context_t *ctx);
const char *app_context_get_webserver_port(app_context_t *ctx);
const char *app_context_get_banner(app_context_t *ctx);
int app_context_get_argc(app_context_t *ctx);
char **app_context_get_argv(app_context_t *ctx);


struct mg_mgr *app_context_get_mongoose_manager(app_context_t *ctx);

int app_context_get_should_stop(app_context_t *ctx);

/* ======================
   OPTIONS (SET)
   ====================== */

void app_context_set_argc(app_context_t *ctx, int argc);
void app_context_set_argv(app_context_t *ctx, char **argv);
int app_context_set_webserver_config_file(app_context_t *ctx, const char *v);
int app_context_set_webserver_ip(app_context_t *ctx, const char *v);
int app_context_set_webserver_port(app_context_t *ctx, const char *v);
int app_context_set_env(app_context_t *ctx, const char *v);
int app_context_set_verbosity(app_context_t *ctx, int v);

/* ======================
   CONTROL
   ====================== */

int app_context_is_running(app_context_t *ctx);
void app_context_stop(app_context_t *ctx);

#endif
