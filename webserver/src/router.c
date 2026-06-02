#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "router.h"
#include "events_handler.h"
#include "renderer/html_renderer.h"
#include "main.h"

struct response {
    char *content;
    char *location;
    char *headers;
};

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

static int handle_set_lang(struct mg_connection *nc,
                           struct mg_http_message *hm) {

    printf("[SET_LANG] Entered handler for URI: %.*s\n", // DEBUG toremove
       (int)hm->uri.len, hm->uri.buf); // DEBUG toremove


    const char *prefix = "/set-lang/";

    if (hm->uri.len < strlen(prefix)) return 0;
    if (strncmp(hm->uri.buf, prefix, strlen(prefix)) != 0) return 0;

    char lang[16];
    sscanf(hm->uri.buf, "/set-lang/%15[^/]", lang);

    if (strcmp(lang, "fr") != 0 &&
        strcmp(lang, "en") != 0 &&
        strcmp(lang, "de") != 0 &&
        strcmp(lang, "uk") != 0 &&
        strcmp(lang, "eo") != 0) {
        return 0;
    }

    char from[512] = "/";

    if (hm->query.len > 0) {
        mg_http_get_var(&hm->query, "from", from, sizeof(from));
    }

    char new_location[512];

    if (from[0] != '/') {
        snprintf(new_location, sizeof(new_location), "/%s/", lang);
    } else {
        char *second_slash = strchr(from + 1, '/');

        if (second_slash) {
            snprintf(new_location, sizeof(new_location),
                     "/%s%s", lang, second_slash);
        } else {
            snprintf(new_location, sizeof(new_location),
                     "/%s/", lang);
        }
    }

    printf("Lang switch: %s -> %s\n", from, new_location);

    // TODO retirer la langue de l'url de redirection

    printf("[SET_LANG] Redirecting to: %s\n", new_location); // DEBUG toremove


    // mg_http_reply(nc, 200,
    //                "Content-Type: text/html; charset=utf-8\r\nSet-Cookie: clio-lang=%s; Path=/; HttpOnly\r\n",
    //                lang,
    //                "%.*s",
    //                (int)strlen(response), response);

    // return 1;

    mg_printf(nc,
    "HTTP/1.1 302 Found\r\n"
    "Location: %s\r\n"
    "Content-Length: 0\r\n"
    "\r\n",
    new_location);


    return 1;

}


static int get_lang_from_url(const char *url, char **lang) {
    if (url[0] != '/') {
        return -1;
    }

    const char *start = url + 1;
    const char *end = strchr(start, '/');

    if (!end) {
        end = start + strlen(start);
    }

    size_t len = end - start;

    if (len == 0 || len >= 16) {
        return -1;
    }

    char lang_buf[16];
    memcpy(lang_buf, start, len);
    lang_buf[len] = '\0';

    // Validation stricte
    if (strcmp(lang_buf, "fr") != 0 &&
        strcmp(lang_buf, "en") != 0 &&
        strcmp(lang_buf, "de") != 0 &&
        strcmp(lang_buf, "uk") != 0 &&
        strcmp(lang_buf, "eo") != 0) {
        return -1;
    }

    *lang = strdup(lang_buf);
    return 0;
}

int remove_lang_from_url(char *url) {
    if (url[0] != '/') {
        return -1;
    }

    const char *start = url + 1;
    const char *end = strchr(start, '/');

    if (!end) {
        url[0] = '/';
        url[1] = '\0';
        return 0;
    }

    size_t lang_len = end - start;

    if (lang_len == 0 || lang_len >= 16) {
        return -1;
    }

    memmove(url, end, strlen(end) + 1);
    return 0;
}

static char* handle_dynamic(struct mg_connection *nc, struct mg_http_message*hm) {
// static struct response* handle_dynamic(struct mg_connection *nc,
                        //    struct mg_http_message *hm) {

    char url[256];

    if (hm->uri.len >= sizeof(url)) {
        return NULL;
    }

    snprintf(url, sizeof(url), "%.*s",
             (int) hm->uri.len,
             hm->uri.buf);

    char *lang = NULL;
    // get_cookie_value(hm, "clio-lang", &lang); // système antérieur, à remplacer par une gestion par handler
    // nouveau système : la langue est indiquée dans l'URL de la route, en deuxième position après le slash initial, ex: /fr/mon-url, /en/mon-url, etc. Si aucune langue n'est indiquée, on considère que c'est du français par défaut.

    if (get_lang_from_url(url, &lang) != 0) {
        printf("Failed to extract language from URL: %s\n", url);
        return NULL;
    }

    if (!lang) {
        lang = strdup("fr"); // fallback propre
    }


    // DEBUG FUNC : if (print_request(nc, hm)) != 0) return NULL;

    char *media = "desktop"; // TODO la prise en compte du type d'appareil du client*

    if (remove_lang_from_url(url) != 0) {
        printf("Failed to remove language from URL: %s\n", url);
        free(lang);
        return NULL;
    } else {
        printf("URL after removing language: %s\n", url);
    }

    char *response = compose_page(url, NULL, lang, "<TODOstyle_sheet>");
    free(lang);

    if (!response) {
        return NULL;
    }

    // struct response *resp = malloc(sizeof(struct response));
    // if (!resp) {
    //     free(response);
    //     return NULL;
    // }
    // resp->content = response;
    // resp->headers = NULL;
    // resp->location = NULL;
    // return resp;

    return response;
}

void router_dispatch(struct mg_connection *nc,
                     struct mg_http_message *hm, app_context_t *ctx) {

    printf("[DEBUG] Method: %.*s\n", (int)hm->method.len, hm->method.buf);

    printf("[ROUTER] Incoming request: %.*s\n", // DEBUG toremove
       (int)hm->uri.len, hm->uri.buf); // DEBUG toremove


    if (handle_favicon(nc, hm)) return;
    // if (handle_set_lang(nc, hm)) return;
    if (handle_set_lang(nc, hm)) {
    printf("[ROUTER] handle_set_lang handled request\n");
    return;
}



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
