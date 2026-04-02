#include "../../common/dependencies/mongoose/mongoose.h"
#include "../../common/src/utils/utils.h"
#include "../../common/src/prompts/prompts.h"
#include "events_handler.h"
#include "html_renderer.h"
#include "main.h"
#include <string.h>

static struct mg_mgr mgr;

typedef void (*route_handler_t)(struct mg_connection *, struct mg_http_message *);

struct route {
    const char *uri;
};

/* Fonction fondamentale */
int run_server(app_context_t *ctx) {
    mg_mgr_init(&mgr); /* Initialize Mongoose event manager */

    char addr[64];

    if (ctx == NULL || ctx->options == NULL) {
        fprintf(stderr, "Invalid context\n");
        return 1;
    }

    snprintf(addr, sizeof(addr), "%s:%s", ctx->options->webserver_ip, ctx->options->webserver_port);

    struct mg_connection *nc =
        mg_http_listen(&mgr, addr, events_handler, ctx);

    if (!nc)
        return 1;

    ok_prompt("Listening on %s", addr);

    for (;;)
        mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
    return 0;
}

int main(int argc, char *argv[]) {
    server_options_t options;
    variables_t variables;

    app_context_t *ctx = malloc(sizeof(app_context_t));
        if (!ctx) {
        perror("malloc");
        return 1;
    }
    ctx->options = &options;
    ctx->vars = &variables;

    if (init(argc, argv, ctx) != 0) {
        return 1;
    }

    run_server(ctx);
    return 0;
}