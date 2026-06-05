#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include "mongoose.h"

typedef struct app_context app_context_t;
typedef struct server_options server_options_t;
typedef struct variables variables_t;


/* Lifecycle */
app_context_t *app_context_create(void);
void app_context_destroy(app_context_t *ctx);

/* Getters (accès contrôlé) */
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


/* Setters (accès contrôlé) */
uint8_t app_context_set_webserver_ip(app_context_t *ctx, const char *ip);
uint8_t app_context_set_webserver_port(app_context_t *ctx, const char *port);
uint8_t app_context_set_banner(app_context_t *ctx, const char *banner);
uint8_t app_context_set_webserver_config_file(app_context_t *ctx, const char *config_file);
uint8_t app_context_set_verbosity(app_context_t *ctx, int verbose);
uint8_t app_context_set_env(app_context_t *ctx, const char *env);
uint8_t app_context_set_should_stop(app_context_t *ctx, int should_stop);

/* Server state */
int app_context_is_running(app_context_t *ctx);
void app_context_stop(app_context_t *ctx);

#endif
