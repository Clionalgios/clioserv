#include "actions.h"
#include "init.h"
#include "server.h"
#include "../router.h" // TODO : renommer en http_handler
#include "context.h"
#include "app_types.h"

/* ======================
   CORE ACTIONS
   ====================== */

int action_init(app_context_t *ctx) {
    return init(
        app_context_get_argc(ctx),
        app_context_get_argv(ctx),
        ctx
    );
}


int action_start(app_context_t *ctx) {
    return server_start(ctx);
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

    return http_handle_request((struct mg_connection *)ev->nc,
                               (struct mg_http_message *)ev->hm, ctx);
}
