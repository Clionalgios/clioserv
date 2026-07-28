#include "actions.h"
#include "init.h"
#include "server.h"
// #include "../router.h" // TODO : renommer en http_handler
#include "context.h"
#include "app_types.h"
#include "events.h"

/* ======================
   CORE ACTIONS
   ====================== */

int action_init(app_context_t *app_app_ctx, app_event_data_t *event) {
    return init(
        app_context_get_argc(app_app_ctx),
        app_context_get_argv(app_app_ctx),
        app_app_ctx
    );
}

int action_start(app_context_t *app_ctx, app_event_data_t *event) {
    return server_start(app_ctx);
}

int action_stop(app_context_t *app_ctx, app_event_data_t *event) {
    server_stop(app_ctx);
    return 0;
}

int action_fail(app_context_t *app_ctx, app_event_data_t *event) {
    return -1;
}

/* ======================
   HTTP
   ====================== */

int handle_http_request(app_context_t *app_ctx, app_event_data_t *event) {
    if (!event) {
        return -1;
    }

    app_http_event_t *ev = &event->data.http;

    if (!&ev->nc || !&ev->hm)
        return -1;

    return http_handle_request(ev->nc, ev->hm, app_ctx);
}
