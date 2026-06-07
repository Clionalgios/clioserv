#ifndef ACTIONS_H
#define ACTIONS_H

#include "app.h"

int action_init(app_context_t *ctx);
int action_start(app_context_t *ctx);
int action_run(app_context_t *ctx);
int action_stop(app_context_t *ctx);
int action_fail(app_context_t *ctx);

int handle_http_request(app_context_t *ctx);

#endif
