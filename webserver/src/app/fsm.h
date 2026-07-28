#ifndef FSM_H
#define FSM_H

#include "context.h"
#include "app_types.h"

int fsm_handle_event(app_context_t *ctx, app_event_data_t *event);

#endif
