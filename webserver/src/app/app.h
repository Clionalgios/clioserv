#ifndef APP_H
#define APP_H

#include "fsm.h"
#include "context.h"

int app_run_lifecycle(int argc, char **argv);

int app_step(app_context_t *ctx, app_event_data_t *event);

#endif
