#include "server.h"
#include "../context/context.h"
#include "../events_handler.h"

uint8_t run_server(app_context_t *ctx) {

    mg_mgr_init(app_context_get_mongoose_manager(ctx));

    char addr[64];
    server_options_t *opt = app_context_get_options(ctx);

    snprintf(addr, sizeof(addr), "%s:%s",
         app_context_get_webserver_ip(ctx),
         app_context_get_webserver_port(ctx));

    struct mg_connection *nc =
        mg_http_listen(app_context_get_mongoose_manager(ctx), addr, events_handler, ctx);

    if (!nc)
        return 1;

    ok_prompt("Listening on %s", addr);

    while (app_context_is_running(ctx)) {
        mg_mgr_poll(app_context_get_mongoose_manager(ctx), 1000);
    }

    return 0;
}
