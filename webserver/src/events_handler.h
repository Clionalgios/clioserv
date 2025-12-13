#ifndef EVENTS_HANDLER_H
#define EVENTS_HANDLER_H

struct mg_connection;
struct mg_http_message;

int match_uri(const char *buf, size_t len, const char *pattern);
void events_handler(struct mg_connection *nc, int ev, void *ev_data);

#endif