#ifndef APP_H
#define APP_H

#include "context.h"

int app_run(int argc, char **argv);

int app_init(app_context_t *ctx, int argc, char **argv);
int app_start(app_context_t *ctx);
int app_run_loop(app_context_t *ctx);
void app_shutdown(app_context_t *ctx);

#endif
