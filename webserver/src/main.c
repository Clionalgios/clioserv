#include "../../common/dependencies/mongoose/mongoose.h"
#include "../../common/src/utils/utils.h"
#include "html_renderer.h"
#include <stdio.h>
#include <string.h>

static struct mg_mgr mgr;

typedef void (*route_handler_t)(struct mg_connection *, struct mg_http_message *);

struct route {
    const char *uri;
};


// Définition des routes sous forme de tableau
static const struct route routes[] = {
    {"/"},
    {NULL}
};

static const char *s_http_port = "127.0.0.1:8181";

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *http_message = (struct mg_http_message *) ev_data;
        struct mg_str *uri = &http_message->uri;

        char *response = NULL;
        char *language = NULL;
        char *media = NULL;
        char *url = malloc(uri->len + 1);
        if (url == NULL) {
            mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Erreur interne du serveur");
            return;
        }
        memcpy(url, uri->buf, uri->len);
        url[uri->len] = '\0';

        get_cookie_value(http_message, "clio-lang", &language);

        // ( à enfermer plus tard dans une fonction de logging)
        uint32_t ip = ntohl(*(uint32_t *) &nc->rem.ip);
        printf("Langue demandée : %s\n", language ? language : "Aucune");
        printf("Nouvelle connexion :\n");
        // printf("  URI : %.*s\n", (int) uri.len, uri.buf);
        printf("  IP source : %d.%d.%d.%d\n", nc->rem.ip[0], nc->rem.ip[1], nc->rem.ip[2], nc->rem.ip[3]);
        printf("  Port source : %d\n", ntohs(nc->rem.port));
        printf("  Méthode : %.*s\n", (int) http_message->method.len, http_message->method.buf);
        printf("  Protocole : %.*s\n", (int) http_message->proto.len, http_message->proto.buf);
        printf("  En-têtes :\n");
        for (int i = 0; i < MG_MAX_HTTP_HEADERS && http_message->headers[i].name.len > 0; i++) {
            printf("    %.*s: %.*s\n", (int) http_message->headers[i].name.len, http_message->headers[i].name.buf,
                   (int) http_message->headers[i].value.len, http_message->headers[i].value.buf);
        }

        response = compose_page(url, media, language, "<TODOstyle_sheet>");
        free(url);
        if (response == NULL) {
            mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Erreur interne du serveur (page non composable)");
            printf("Admin tip : vérifiez d'être dans le répertoire ./clioserv pour lancer l'exécutable dans ./clioserv/exploitation TODO DESTROY\n");

            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Répertoire actuel : %s\n", cwd);
            } else {
                perror("getcwd() error");
            }

            return;
        }


        mg_http_reply(nc, 200, "Content-Type: text/html\r\n", response);
        free(response);
    }
}

int main(void) {
    struct mg_connection *nc;

    mg_mgr_init(&mgr);

    nc = mg_http_listen(&mgr, s_http_port, ev_handler, NULL);
    if (nc == NULL) {
        fprintf(stderr, "Échec de la connexion au port %s\n", s_http_port);
        return 1;
    }

    printf("Webserver - Serveur HTTP démarré sur le port %s\n", s_http_port);

    while (1) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
    return 0;
}
