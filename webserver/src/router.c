#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "router.h"
#include "events_handler.h"
#include "renderer/html_renderer.h"
#include "main.h"

static void reply_500(struct mg_connection *nc) {
    mg_http_reply(nc,
                  500,
                  "Content-Type: text/plain\r\n",
                  "Internal Server Error");
}

static char* handle_banner(struct mg_connection *nc,
                         struct mg_http_message *hm) {

    if (!match_uri(&hm->uri, "/banner.txt"))
        return NULL;

    char* banner = get_server_banner();

    if (banner == NULL) {
        return NULL;
    }

    return banner;
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

static char* handle_dynamic(struct mg_connection *nc,
                           struct mg_http_message *hm) {

    char url[256];

    if (hm->uri.len >= sizeof(url)) {
        return NULL;
    }

    snprintf(url, sizeof(url), "%.*s",
             (int) hm->uri.len,
             hm->uri.buf);

    // TODO: récupérer langue proprement plus tard
    const char *lang = "fr";

    char *response = compose_page(url, NULL, lang, "<TODOstyle_sheet>");

    if (!response) {
        return NULL;
    }

    return response;
}

void router_dispatch(struct mg_connection *nc,
                     struct mg_http_message *hm, app_context_t *ctx) {

    if (handle_favicon(nc, hm)) return;

    char* content = NULL;

    content = handle_banner(nc, hm);
    if (content == NULL) {
        content = handle_dynamic(nc, hm);
        if (content == NULL) {
            reply_500(nc);
            return;
        }
    }

    char* headers = snprintf(NULL, 0,
        "Server: %s\r\nContent-Type: text/html; charset=utf-8\r\n",
        *ctx->vars->banner) + 1;


    mg_http_reply(nc, 200,
                  headers,
                  "%.*s\n",
                  (int)strlen(content), content);

    free(content);
    free(headers);
    return 1;
}
