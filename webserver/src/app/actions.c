#include "actions.h"
#include "init.h"
#include "server.h"
#include "http_handler.h"
#include "context.h"

/* ======================
   CORE ACTIONS
   ====================== */

int action_init(app_context_t *ctx) {
    return init(0, NULL, ctx);
}

int action_start(app_context_t *ctx) {
    return server_start(ctx);
}

int action_run(app_context_t *ctx) {
    return server_running(ctx);
}

int action_stop(app_context_t *ctx) {
    server_stop(ctx);
    return 0;
}

int action_fail(app_context_t *ctx) {
    return -1;
}

/* ======================
   HTTP
   ====================== */

int handle_http_request(app_context_t *ctx) {
    app_http_event_t *ev = app_context_get_http_event(ctx);

    if (!ev || !ev->nc || !ev->hm)
        return -1;

    return http_handle_request(ev->nc, ev->hm, ctx);
}
