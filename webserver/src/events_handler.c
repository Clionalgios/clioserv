#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "events_handler.h" 
#include "context.h"
#include "app_types.h"
#include "fsm.h"
#include "app.h"
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


void events_handler(struct mg_connection *c, int ev, void *ev_data) {
    app_context_t *ctx = (struct app_context_t *) c->fn_data;

    switch (ev) {
        case MG_EV_HTTP_MSG: {
            struct mg_http_message *hm = (struct mg_http_message *) ev_data;

            app_http_event_t http_ev = {
                .nc = c,
                .hm = hm
            };

            app_context_set_http_event(ctx, &http_ev);
            app_dispatch(ctx, APP_EVENT_HTTP_REQUEST);
            break;
        }

        default:
            // ✅ NE RIEN FAIRE
            break;
    }
}
