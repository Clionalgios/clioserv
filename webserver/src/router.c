#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "router.h"
#include "events_handler.h"
#include "renderer/html_renderer.h"

static void reply_500(struct mg_connection *nc) {
    mg_http_reply(nc,
                  500,
                  "Content-Type: text/plain\r\n",
                  "Internal Server Error");
}

static int handle_banner(struct mg_connection *nc,
                         struct mg_http_message *hm) {


    if (!match_uri(&hm->uri, "/banner"))
        return 0;

    mg_http_reply(nc, 200,
                "Content-Type: text/plain\r\n",
                "%s",
                get_server_banner());


    return 1;
}

static int handle_favicon(struct mg_connection *nc,
                          struct mg_http_message *hm) {

    if (!match_uri(&hm->uri, "/favicon.ico"))
        return 0;

    struct mg_http_serve_opts opts = {
        .extra_headers = "Content-Type: image/x-icon\r\n"
    };

    mg_http_serve_file(nc, hm,
        "./website_assets/favicon.ico",
        &opts);

    return 1;
}

static void handle_dynamic(struct mg_connection *nc,
                           struct mg_http_message *hm) {

    char url[256];

    if (hm->uri.len >= sizeof(url)) {
        reply_500(nc);
        return;
    }

    snprintf(url, sizeof(url), "%.*s",
             (int) hm->uri.len,
             hm->uri.buf);

    // TODO: récupérer langue proprement plus tard
    const char *lang = "fr";

    char *response = compose_page(url, NULL, lang, "<TODOstyle_sheet>");

    if (!response) {
        reply_500(nc);
        return;
    }

    mg_http_reply(nc,
                  200,
                  "Content-Type: text/html\r\n",
                  "%s",
                  response);

    free(response);
}

void router_dispatch(struct mg_connection *nc,
                     struct mg_http_message *hm) {

    if (handle_banner(nc, hm)) return;
    if (handle_favicon(nc, hm)) return;

    handle_dynamic(nc, hm);
}
