#include "server.h"
#include "app.h"
#include "fsm.h"
#include "../context/context.h"
#include "../events_handler.h"

uint8_t server_start(app_context_t *ctx) {
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
    return 0;

}

uint8_t server_stop(app_context_t *ctx) {
    app_context_stop(ctx);
    return 0;
}

uint8_t server_running(app_context_t *ctx) {
    while (!app_context_get_should_stop(ctx)) {
        mg_mgr_poll(app_context_get_mongoose_manager(ctx), 1000);

        app_dispatch(ctx, APP_EVENT_TICK);
    }


    return 0;
}
