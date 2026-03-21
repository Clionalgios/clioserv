#ifndef EVENTS_HANDLER_H
#define EVENTS_HANDLER_H

#include <mongoose.h>

struct mg_connection;
struct mg_http_message;

int match_uri(struct mg_str *uri, const char *pattern);
void events_handler(struct mg_connection *nc, int ev, void *ev_data);

#endif