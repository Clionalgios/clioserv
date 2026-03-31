#ifndef ROUTER_H
#define ROUTER_H

#include "mongoose.h"
#include "main.h"

void router_dispatch(struct mg_connection *nc,
                     struct mg_http_message *hm, app_context_t *ctx);

#endif
