#include "../../common/dependencies/mongoose/mongoose.h"
#include "../../common/src/utils/utils.h"
#include <stdio.h>
#include <string.h>


// On déclare le gestionnaire de connexion comme variable statique globale au fichier main.c
static struct mg_mgr mgr;

static char *server_ip = "127.0.0.1";
static char *server_port = "8080";
static char *web_server = "http://127.0.0.1:8181";
static char *api_server = "http://127.0.0.1:8282";

// Structure pour passer les données de réponse
struct fetch_result {
    char *body;
    size_t body_len;
    int done;
};

const char *head_bloc_content = "<!DOCTYPE html>\n"
                                 "<html lang=\"fr\">\n"
                                 "<head>\n"
                                 "    <meta charset=\"UTF-8\">\n"
                                 "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                 "    <title>{{{title}}}</title>\n"
                                 "</head>\n"
                                 "{{{body}}}\n"
                                 "</html>";

// Handler utilisé par mg_http_connect
static void fetch_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    struct fetch_result *res = (struct fetch_result *) c->fn_data;

    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *http_message = (struct mg_http_message *) ev_data;
        res->body = malloc(http_message->body.len + 1);
        if (res->body) {
            memcpy(res->body, http_message->body.buf, http_message->body.len);
            res->body[http_message->body.len] = '\0';
            res->body_len = http_message->body.len;
        }
        res->done = 1;
        c->is_closing = 1;
    } else if (ev == MG_EV_CLOSE) {
        res->done = 1;
    }
}

static void not_found(struct mg_connection *nc, struct mg_http_message *http_message) {
    mg_http_reply(nc, 404, "", "<!DOCTYPE html><html><body><h1>404 Not Found</h1>"
                               "<p>Anormal behavior : webserver is not available.</p>"
                               "<p>Please contact admin at lglanois@gmail.com</p></body></html>",
                  (int) http_message->uri.len, http_message->uri.buf);
    printf("hello\n");
}

static void handle_api_requests(struct mg_connection *nc, struct mg_http_message *http_message) {
    mg_http_reply(nc, 200, "Content-Type: application/json\r\n",
                 "{\"message\": \"API Route\"}");
}

// Structure pour retourner à la fois le résultat et un message d'erreur
struct request_result {
    struct fetch_result result;
    char *error;
};

// Routage et récupération de la réponse du serveur web
static struct request_result request_to_webserver(char *base_url, struct mg_str *uri, struct mg_http_message **http_message) {
    struct request_result req_res = {0};
    struct mg_http_message *request = *http_message;

    // Construction de l'URL à partir des arguments
    char url[256];
    snprintf(url, sizeof(url), "%s%.*s", base_url, (int) uri->len, uri->buf);

    // Envoi de la requête HTTP
    struct mg_connection *c = mg_http_connect(&mgr, url, fetch_ev_handler, &req_res.result);
    if (c == NULL) {
        req_res.result.done = 1;
        req_res.error = strdup("Erreur de connexion HTTP");
        return req_res;
    }

    // Début de la requête avec la même méthode que la requête originale
    mg_printf(c, "%.*s %.*s HTTP/1.1\r\n", 
              (int) request->method.len, request->method.buf,
              (int) uri->len, uri->buf);
    
    // Transférer les en-têtes de l'utilisateur
    for (int i = 0; i < MG_MAX_HTTP_HEADERS && request->headers[i].name.len > 0; i++) {
        // Ignorer l'en-tête Host car nous le définissons nous-mêmes
        if (mg_strcmp(request->headers[i].name, mg_str("Host")) == 0) {
            continue;
        }
        mg_printf(c, "%.*s: %.*s\r\n", 
                 (int) request->headers[i].name.len, request->headers[i].name.buf,
                 (int) request->headers[i].value.len, request->headers[i].value.buf);
    }
    
    // Ajouter l'en-tête Host pour le serveur cible
    mg_printf(c, "Host: %s\r\n", base_url + strlen("http://"));
    mg_printf(c, "Connection: close\r\n\r\n");
    
    // Transférer le corps de la requête si présent
    if (request->body.len > 0) {
        mg_send(c, request->body.buf, request->body.len);
    }

    // Attente de la réponse
    while (!req_res.result.done) {
        mg_mgr_poll(&mgr, 100);
    }

    if (req_res.result.body == NULL) {
        req_res.error = strdup("Réponse vide ou erreur lors de la récupération de la réponse");
    } else {
        req_res.error = NULL;
    }

    return req_res;
}

static void reply(struct mg_connection *nc, int status, const char *content_type, const char *body) {
    mg_http_reply(nc, status, content_type, "%s", body);
}

static void set_response_in_html_overlay(struct request_result *res) {
    if (res->result.body == NULL) {
        return;
    }

    // Charger le template HTML
    char *html_template = strdup(head_bloc_content);
    if (html_template == NULL) {
        return;
    }

    // Remplacer les placeholders dans le template
    char *final_html = NULL;
    size_t final_size = strlen(html_template) + strlen(res->result.body) + 1; // +1 pour le null terminator
    final_html = malloc(final_size);
    if (final_html == NULL) {
        free(html_template);
        return;
    }

    // Simple remplacement des placeholders
    char *pos = strstr(html_template, "{{{body}}}");
    if (pos) {
        size_t before_len = pos - html_template;
        snprintf(final_html, final_size, "%.*s%s%s", 
                 (int) before_len, html_template, 
                 res->result.body, 
                 pos + strlen("{{{body}}}"));
    } else {
        snprintf(final_html, final_size, "%s", html_template);
    }

    // On récupère la balise <title> placée dans le body
    char *title = strstr(res->result.body, "<title>"); // Assuming the title is in the body
    if (title) {
        title += strlen("<title>");
        char *end_title = strstr(title, "</title>");
        if (end_title) {
            *end_title = '\0'; // Temporarily terminate the string
            char *title_placeholder = "{{{title}}}";
            char *title_pos = strstr(final_html, title_placeholder);
            if (title_pos) {
                size_t before_title_len = title_pos - final_html;
                size_t after_title_len = strlen(final_html) - (before_title_len + strlen(title_placeholder));
                size_t new_size = before_title_len + strlen(title) + after_title_len + 1;
                char *new_html = malloc(new_size);
                if (new_html) {
                    snprintf(new_html, new_size, "%.*s%s%.*s", 
                             (int) before_title_len, final_html, 
                             title, 
                             (int) after_title_len, title_pos + strlen(title_placeholder));
                    free(final_html);
                    final_html = new_html;
                }
            }
            *end_title = '<'; // Restore the original character
        }
    }
    

    free(html_template);
    free(res->result.body);
    res->result.body = final_html;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    char *language = NULL;
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *http_message = (struct mg_http_message *) ev_data;
        struct mg_str uri = http_message->uri;
        struct request_result response = {0};


        // ( à enfermer plus tard dans une fonction de logging)
        uint32_t ip = ntohl(*(uint32_t *) &nc->rem.ip);
        printf("new connexion :\n");
        printf("  url : %.*s\n", (int) uri.len, uri.buf);
        printf("  source ip : %d.%d.%d.%d\n", nc->rem.ip[0], nc->rem.ip[1], nc->rem.ip[2], nc->rem.ip[3]);
        printf("  source port : %d\n", ntohs(nc->rem.port));
        printf("  method : %.*s\n", (int) http_message->method.len, http_message->method.buf);
        printf("  protocol : %.*s\n", (int) http_message->proto.len, http_message->proto.buf);
        printf("  headers :\n");
        for (int i = 0; i < MG_MAX_HTTP_HEADERS && http_message->headers[i].name.len > 0; i++) {
            printf("    %.*s: %.*s\n", (int) http_message->headers[i].name.len, http_message->headers[i].name.buf,
                   (int) http_message->headers[i].value.len, http_message->headers[i].value.buf);
        }
        if (http_message->body.len > 0) {
            printf("  body : %.*s\n", (int) http_message->body.len, http_message->body.buf);
        } else {
            printf("  no body found\n");
        }

        // Si la requête commence par /api, la rediriger vers le serveur API
        if (mg_strcmp(uri, mg_str("/api")) == 0) {
            response = request_to_webserver(api_server, &uri, &http_message);

            if (response.error || !response.result.body) {
                reply(nc, 500, "Content-Type: application/json\r\n", response.error);
            } else if (response.result.body) {
                reply(nc, 200, "Content-Type: application/json\r\n", response.result.body);
            } else {
                reply(nc, 500, "", "Upstream error");
            }
        } else {
            get_cookie_value(http_message, "clio-lang", &language);
            if (language == NULL) {
                language = "fr";
            }

            // On passe : l'adresse du serveur web, l'URI de la requête et le message HTTP complet
            response = request_to_webserver(web_server, &uri, &http_message);
            // set_response_in_html_overlay(&response);

            if (response.error || !response.result.body) {
                mg_http_reply(nc, 502, "", "Bad Gateway");
                return;
            }

            if (response.result.body) {
                // not_found(nc, http_message);
                // On emballe la réponse dans le template HTML head bloc content
                set_response_in_html_overlay(&response);
                mg_http_reply(nc, 200, "Content-Type: text/html\r\n", "%s", response.result.body);
                free(response.result.body);
                return;
            } else {
                mg_http_reply(nc, 502, "", "Upstream error");
            }
        }

        // Si en définitive, la route n'existe pas, 404
        not_found(nc, http_message);
        printf("TODO : logs\n");

    }
}

void parse_args(int argc, char *argv[], char **ip, char **port) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            *port = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            *ip = argv[i + 1];
            i++;
        }
    }
}

int main(int argc, char *argv[]) {
    struct mg_connection *nc;

    parse_args(argc, argv, &server_ip, &server_port);

    mg_mgr_init(&mgr);

    // Adresse d'écoute = serveur IP + port
    char server_ip_port[64];
    snprintf(server_ip_port, sizeof(server_ip_port), "%s:%s", server_ip, server_port);

    // Liaison du gestionnaire à un port spécifique pour écouter les requêtes entrantes
    nc = mg_http_listen(&mgr, server_ip_port, ev_handler, NULL);
    if (nc == NULL) {
        fprintf(stderr, "Échec de la connexion au port %s\n", server_ip_port);

        return 1;
    }

    printf("Routeur - Serveur HTTP démarré sur le port %s\n", server_ip_port);

    while (1) {
        mg_mgr_poll(&mgr, 1000); // Polling pour les événements
    }

    mg_mgr_free(&mgr);
    return 0;
}
