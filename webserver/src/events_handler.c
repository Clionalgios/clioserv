#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "events_handler.h" 
#include "main.h"
#include <mongoose.h>

/*
 * match_uri(buf, pattern)
 *
 * Compare l'URI (buf) avec un motif littéral `pattern` (NUL-terminated).
 * - ignore la query string et le fragment (tout après '?' ou '#')
 * - effectue une comparaison exacte du chemin (pas de wildcard)
 *
 * Retourne 1 si égal (match), 0 sinon.
 */
int match_uri(struct mg_str *uri, const char *pattern) {
    size_t end = 0;

    while (end < uri->len) {
        if (uri->buf[end] == '?' || uri->buf[end] == '#') break;
        end++;
    }

    size_t plen = strlen(pattern);
    if (plen != end) return 0;

    return (memcmp(uri->buf, pattern, plen) == 0) ? 1 : 0;
}


void events_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev != MG_EV_HTTP_MSG) return;

    app_context_t *ctx = (app_context_t *) nc->fn_data;

    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    if (mg_strcmp(hm->method, mg_str("GET")) != 0) {
        mg_http_reply(nc, 405,
                      "Content-Type: text/plain\r\n",
                      "Method Not Allowed\n");
        return;  // ✅ PAS return 1
    }

    router_dispatch(nc, hm, ctx);
}
