#ifndef SERVER_H
#define SERVER_H

#include "context.h"

uint8_t server_start(app_context_t *ctx);
uint8_t server_stop(app_context_t *ctx);
uint8_t server_running(app_context_t *ctx);

#endif /* SERVER_H */