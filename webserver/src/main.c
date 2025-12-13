#include "../../common/dependencies/mongoose/mongoose.h"
#include "../../common/src/utils/utils.h"
#include "../../common/src/prompts/prompts.h"
#include "events_handler.h"
#include "html_renderer.h"
#include <string.h>

#define RED  "\x1B[31m"
#define RESET  "\x1B[0m"
#define STYLE_BOLD  "\x1B[1m"


static struct mg_mgr mgr;

typedef void (*route_handler_t)(struct mg_connection *, struct mg_http_message *);

struct route {
    const char *uri;
};

static const char *s_http_port = "127.0.0.1:8181";

/* Fonction fondamentale */
int run_server(void) {
    mg_mgr_init(&mgr); /* Initialize Mongoose event manager */

    struct mg_connection *nc =
        mg_http_listen(&mgr, s_http_port, events_handler, NULL);

    if (!nc)
        return 1;

    ok_prompt("Listening on %s", s_http_port);

    for (;;)
        mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
    return 0;
}

// TODO: déplacer cette fonction dans init.c
int ensure_assets_directory(void) {
    const char *path = "./website_assets";
    struct stat st;

    
    if (stat(path, &st) != 0) { /* Vérifie l'existence du dossier */
        error_prompt("folder \"%s\" not found", path);

        info_prompt("you may not be in the correct working directory");
        info_prompt("default expected path: clioserv/exploitation");
        info_prompt("ensure that the folder contains \"%s/\"", path);
        print_current_directory();
        return 0;
    }

    if (!S_ISDIR(st.st_mode)) { /* Vérifie qu'il s'agit bien d'un répertoire */
        error_prompt("\"%s\" exists but is not a directory", path);
        info_prompt("recommended fix: reclone the repository");
        return 0;
    }

    ok_prompt("assets directory \"%s\" detected", path);
    return 1;
}

// TODO: déplacer cette fonction dans init.c
void print_current_directory(void) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        info_prompt("current directory: %s", cwd);
    } else {
        error_prompt(RED STYLE_BOLD"current folder unavailable"RESET);
    }
}

int main(int argc, char *argv[]) {
    init(argc, &argv);

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