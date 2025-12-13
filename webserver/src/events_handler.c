#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "events_handler.h" 
#include <mongoose.h>

/*
 * match_uri(buf, len, pattern)
 *
 * Compare l'URI (buf,len) avec un motif littéral `pattern` (NUL-terminated).
 * - ignore la query string et le fragment (tout après '?' ou '#')
 * - effectue une comparaison exacte du chemin (pas de wildcard)
 *
 * Retourne 1 si égal (match), 0 sinon.
 */
int match_uri(const char *buf, size_t len, const char *pattern) {
    /* détermine la longueur effective de l'URI (avant '?' ou '#') */
    size_t end = 0;
    while (end < len) {
        if (buf[end] == '?' || buf[end] == '#') break;
        end++;
    }

    size_t plen = strlen(pattern);
    if (plen != end) return 0;               /* tailles différentes => pas match */
    if (end == 0 && plen == 0) return 1;     /* both empty */

    /* comparaison binaire sur la portion utile */
    return (memcmp(buf, pattern, plen) == 0) ? 1 : 0;
}

void events_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev != MG_EV_HTTP_MSG) return;

    struct mg_http_message *hm = ev_data;

    // --- FAVICON -----------------------------------------------------
    if (match_uri(hm->uri.buf, hm->uri.len, "/favicon.ico")) {
        struct mg_http_serve_opts opts = {
            .extra_headers = "Content-Type: image/x-icon\r\n"
        };
        mg_http_serve_file(nc, hm, "./website_assets/favicon.ico", &opts);
        return;  // Ne pas poursuivre
    }
    // ----------------------------------------------------------------

    // À partir d’ici : logique des pages normales
    char *url = malloc(hm->uri.len + 1);
    if (!url) {
        mg_http_reply(nc, 500, "", "Internal server error");
        return;
    }

    // TODO: parse URL into url/media/language

    char *language = NULL;
    get_cookie_value(hm, "clio-lang", &language);

    char *response = compose_page(url, NULL, language, "<TODOstyle_sheet>");
    free(url);

    if (!response) {
        mg_http_reply(nc, 500, "", "Internal server error");
        return;
    }

    mg_http_reply(nc, 200, "Content-Type: text/html\r\n", "%s", response);
    free(response);
}