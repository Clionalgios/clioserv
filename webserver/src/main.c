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

int main(int argc, char *argv[]) {
    printf("\n");
    printf("HI\n");
    printf("Total arguments: %d\n", argc);

    // Print each argument
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    if (init(argc, argv) != 0) {
        return 1;
    }

    run_server();
    return 0;
}