#ifndef ROUTER_H
#define ROUTER_H

#include "mongoose.h"

void router_dispatch(struct mg_connection *nc,
                     struct mg_http_message *hm);

#endif
