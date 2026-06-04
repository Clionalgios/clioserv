#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include "mongoose.h"

// /* Forward declarations */
// typedef struct {
//     char *banner;
// } variables_t;

// typedef struct {
//     char *config_file;

//     char *webserver_ip;
//     char *webserver_port;

//     int router_mode;
//     char *router_port;

//     int verbose;
//     int debug;

//     int dry_run;

//     char *log_dir;
//     int disable_logs;

//     char *env;

// } server_options_t;

// typedef struct {
//     server_options_t *options;
//     variables_t *vars;
//     struct mg_mgr mgr;
//     uint8_t running;
// } app_context_t;

typedef struct app_context app_context_t;
typedef struct server_options server_options_t;
typedef struct variables variables_t;

/* Lifecycle */
app_context_t *app_context_create(void);
void app_context_destroy(app_context_t *ctx);

/* Init */
int app_context_init(app_context_t *ctx, int argc, char **argv);

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


/* Setters (accès contrôlé) */
void app_context_set_webserver_ip(app_context_t *ctx, const char *ip);
void app_context_set_webserver_port(app_context_t *ctx, const char *port);
void app_context_set_banner(app_context_t *ctx, const char *banner);
void app_context_set_webserver_config_file(app_context_t *ctx, const char *config_file);
void app_context_set_verbosity(app_context_t *ctx, int verbose);
void app_context_set_env(app_context_t *ctx, const char *env);

/* Server state */
int app_context_is_running(app_context_t *ctx);
void app_context_stop(app_context_t *ctx);

#endif
