#ifndef ACTIONS_H
#define ACTIONS_H

#include "app.h"
#include "context.h"
#include "events.h"

int action_init(app_context_t *ctx, app_event_data_t *event);
int action_start(app_context_t *ctx, app_event_data_t *event);
int action_run(app_context_t *ctx, app_event_data_t *event);
int action_stop(app_context_t *ctx, app_event_data_t *event);
int action_fail(app_context_t *ctx, app_event_data_t *event);

int handle_http_request(app_context_t *ctx, app_event_data_t *event);

#endif
