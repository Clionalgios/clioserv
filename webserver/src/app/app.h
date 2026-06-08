#ifndef APP_H
#define APP_H

#include "fsm.h"
#include "context.h"

int app_run(int argc, char **argv);

/* DISPATCH CENTRAL */
int app_dispatch(app_context_t *ctx, app_event_t event);

#endif
