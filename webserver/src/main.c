// #include "../../common/dependencies/mongoose/mongoose.h"
// #include "../../common/src/utils/utils.h"
// #include "../../common/src/prompts/prompts.h"
// #include "html_renderer.h"
#include "init/init.h"
#include "context.h"
// #include <string.h>
#include "server.h"

uint8_t main(int argc, char *argv[]) {
    server_options_t options;
    variables_t variables;

    app_context_t ctx;
    ctx.options = &options;
    ctx.vars = &variables;
    ctx.running = 1;

    if (init(argc, argv, &ctx) != 0) {
        return 1;
    }

    return run_server(&ctx);
}