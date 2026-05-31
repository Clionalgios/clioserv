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

static int print_request(struct mg_connection *nc,
                         struct mg_http_message *hm) {

    if (&hm->uri != NULL && hm->method.buf != NULL && hm->method.len != 0) {
        printf("Received request: %.*s %.*s\n",
           (int)hm->method.len, hm->method.buf,
           (int)hm->uri.len, hm->uri.buf);
           return 0;
    } else {
        printf("Warning, request received invalid: missing method or URI");
        return -1;
    }
}



// static int handle_set_lang(struct mg_connection *nc,
//                            struct mg_http_message *hm) {

//     const char *prefix = "/set-lang/";

//     // ✅ Vérifier que l'URI match
//     if (hm->uri.len < strlen(prefix)) return 0;
//     if (strncmp(hm->uri.buf, prefix, strlen(prefix)) != 0) return 0;

//     // ✅ Extraire la langue
//     const char *start = hm->uri.buf + strlen(prefix);
//     const char *end = hm->uri.buf + hm->uri.len;

//     if (end > start && *(end - 1) == '/') {
//         end--;
//     }

//     size_t len = end - start;

//     if (len == 0 || len >= 16) return 0;

//     char lang[16];
//     memcpy(lang, start, len);
//     lang[len] = '\0';

//     // ✅ Validation stricte
//     if (strcmp(lang, "fr") != 0 &&
//         strcmp(lang, "en") != 0 &&
//         strcmp(lang, "de") != 0 &&
//         strcmp(lang, "uk") != 0 &&
//         strcmp(lang, "eo") != 0) {
//         return 0;
//     }

//     // ✅ Récupérer le redirect depuis la query
//     char redirect[512] = "/";

//     if (hm->query.len > 0) {
//         char value[512];

//         if (mg_http_get_var(&hm->query, "redirect", value, sizeof(value)) > 0) {
//             snprintf(redirect, sizeof(redirect), "%s", value);
//         }
//     }

//     // ✅ Debug propre
//     printf("Set-Lang: %s | Redirect: %s\n", lang, redirect);

//     char* response = compose_page(redirect, NULL, lang, "<TODOstyle_sheet>");
//     if (!response) {
//         reply_500(nc);
//         return 1;
//     }

//     mg_http_reply(nc, 200,
//                   "Content-Type: text/html; charset=utf-8\r\nSet-Cookie: clio-lang=%s; Path=/; HttpOnly\r\n",
//                   lang,
//                   "%.*s",
//                   (int)strlen(response), response);


//     return 1;
// }



static char* handle_dynamic(struct mg_connection *nc,
                           struct mg_http_message *hm) {

    char url[256];

    if (hm->uri.len >= sizeof(url)) {
        return NULL;
    }

    snprintf(url, sizeof(url), "%.*s",
             (int) hm->uri.len,
             hm->uri.buf);

    char *lang = NULL;
    get_cookie_value(hm, "clio-lang", &lang);

    if (!lang) {
        lang = strdup("fr"); // fallback propre
    }


    // DEBUG FUNC : if (print_request(nc, hm)) != 0) return NULL;

    char *media = "desktop"; // TODO la prise en compte du type d'appareil du client

    char *response = compose_page(url, NULL, lang, "<TODOstyle_sheet>");
    free(lang);

    if (!response) {
        return NULL;
    }

    return response;
}

void router_dispatch(struct mg_connection *nc,
                     struct mg_http_message *hm, app_context_t *ctx) {

    if (handle_favicon(nc, hm)) return;
    // if (handle_set_lang(nc, hm)) return;


    char* content = NULL;

    content = handle_banner(nc, hm);
    if (content == NULL) {
        content = handle_dynamic(nc, hm);
        if (content == NULL) {
            reply_500(nc);
            return;
        }
    }

    // ✅ SAFE BANNER
    const char *banner = "Clioserv";

    if (ctx && ctx->vars && ctx->vars->banner && *ctx->vars->banner) {
        banner = *ctx->vars->banner;
    }

    int headers_len = snprintf(NULL, 0,
        "Server: %s\r\nContent-Type: text/html; charset=utf-8\r\n",
        banner);

    char *headers = malloc(headers_len + 1);
    if (!headers) {
        free(content);
        reply_500(nc);
        return;
    }

    // Charge le header avec le banner (ou la valeur par défaut)
    snprintf(headers, headers_len + 1,
        "Server: %s\r\nContent-Type: text/html; charset=utf-8\r\n",
        banner);

    // Renvoit la page demandée
    mg_http_reply(nc, 200,
                  headers,
                  "%.*s",
                  (int)strlen(content), content);

    free(content);
    free(headers);
}
