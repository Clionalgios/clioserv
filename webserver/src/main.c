#include "../../common/dependencies/mongoose/mongoose.h"
#include "../../common/src/utils/utils.h"
#include "../../common/src/prompts/prompts.h"
#include "html_renderer.h"
#include <stdio.h>
#include <string.h>

#define CLEAR_SCREEN "\x1B[2J\x1B[H"
#define RED  "\x1B[31m"
#define RESET  "\x1B[0m"
#define STYLE_BOLD  "\x1B[1m"


static struct mg_mgr mgr;

typedef void (*route_handler_t)(struct mg_connection *, struct mg_http_message *);

struct route {
    const char *uri;
};


// Définition des routes sous forme de tableau
// static const struct route routes[] = {
//     {"/"},
//     {NULL}
// };

static const char *s_http_port = "127.0.0.1:8181";

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *http_message = (struct mg_http_message *) ev_data;
        struct mg_str *uri = &http_message->uri;

        char *response = NULL;
        char *language = NULL;
        char *media = NULL;

        char *url = malloc(uri->len + 1);

        // setting_favicon(/* sending variables nc, http_message, path and options to the function */);

        // struct mg_http_serve_opts favicon_opts = {
        //     .extra_headers = "Content-Type: image/x-icon\r\n"
        // };
        // const char *path = "./website_assets/favicon.ico";
        // mg_http_serve_file(nc, http_message, path, &favicon_opts);

        if (url == NULL) {
            mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Erreur interne du serveur");
            return;
        }
        memcpy(url, uri->buf, uri->len);
        url[uri->len] = '\0';

        get_cookie_value(http_message, "clio-lang", &language);

        // ( à enfermer plus tard dans une fonction de logging)
        // uint32_t ip = ntohl(*(uint32_t *) &nc->rem.ip); TODO supp
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

int run_server(void) {
    mg_mgr_init(&mgr); /* Initialize Mongoose event manager */

    struct mg_connection *nc =
        mg_http_listen(&mgr, s_http_port, ev_handler, NULL);

    if (!nc)
        return 1;

    ok_prompt("Listening on %s", s_http_port);

    for (;;)
        mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
    return 0;
}

// TODO: placer la présente fonction dans un futur fichier init.c
int ensure_assets_directory(void) {
    struct stat st;

    if (stat("./website_assets", &st) != 0) {
        error_prompt("folder \"./website_assets/\" not found");
        info_prompt("you may not be in the right folder (by default : clioserv/exploitation)");
        info_prompt("check that the working folder contains the folder ./website_assets");
        print_current_directory();
        return 0;
    }

    if (!S_ISDIR(st.st_mode)) {
        error_prompt("\"./website_assets\" exist but isn't a folder");
        info_prompt("best course of action may be to fully reclone the repo");
        return 0;
    }

    ok_prompt("website_assets folder found");
    return 1;
}

// Fonction d'essai. Envisager la suppression plus tard
void print_current_directory(void) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        info_prompt("current directory: %s", cwd);
    } else {
        error_prompt(RED STYLE_BOLD"current folder unavailable"RESET);
    }
}

int main(void) {
    printf(CLEAR_SCREEN);

    // TODO: Start-up banner

    ok_prompt("Initializing Clioserv's webserver...");

    // TODO: load configuration file
    // TODO: parse command-line arguments

    // TODO: externaliser la présente commande dans un futur ensemble init
    if (!ensure_assets_directory()) {
        return 1;
    }

    run_server();
    return 0;
}