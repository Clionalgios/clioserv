#include "context.h"
#include "../events_handler.h"

/* Fonction fondamentale */
uint8_t run_server(app_context_t *ctx) {
    mg_mgr_init(&ctx->mgr); /* Initialize Mongoose event manager */

    char addr[64];

    if (ctx == NULL || ctx->options == NULL) {
        fprintf(stderr, "Invalid context\n");
        return 1;
    }

    snprintf(addr, sizeof(addr), "%s:%s", ctx->options->webserver_ip, ctx->options->webserver_port);

    struct mg_connection *nc =
        mg_http_listen(&ctx->mgr, addr, events_handler, ctx);

    if (!nc)
        return 1;

    ok_prompt("Listening on %s", addr);

    while (ctx->running) {
        mg_mgr_poll(&ctx->mgr, 1000);
    }


    mg_mgr_free(&ctx->mgr);
    return 0;
}