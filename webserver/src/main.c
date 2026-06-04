#include "init/init.h"
#include "context.h"
#include "server.h"

int main(int argc, char *argv[]) {
    app_context_t *ctx = app_context_create();
    if (!ctx) return 1;

    if (init(argc, argv, ctx) != 0) {
        return 1;
    }

    return run_server(ctx);
}