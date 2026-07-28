#include "app.h"
#include "fsm.h"
#include "context.h"
#include "prompts.h"
#include "events.h"
#include "events_handler.h"

int app_step(app_context_t *ctx, app_event_data_t *event) {
    return fsm_handle_event(ctx, event);
}

int app_init(app_context_t *app_ctx) {
    app_event_data_t ev_init = {
        .type = APP_EVENT_INIT,
        .data = NULL
    };

    if (app_step(app_ctx, &ev_init) != 0) {
        return -1;
    } else {
        ok_prompt("Initialization successful.");
        return 0;
    }
}

int app_run_lifecycle(int argc, char **argv) {
    app_context_t *app_ctx = app_context_create();
    if (!app_ctx) {
        error_prompt("Failed to create app_context");
        return 1;
    } else {
        // On transfère le contexte argumentaire du programme à la  
        // structure de contexte interne au programme
        app_context_set_argc(app_ctx, argc);
        app_context_set_argv(app_ctx, argv);
    }

    if (app_init(app_ctx) != 0)
        goto cleanup;

    // app_event_data_t ev_init = {
    //     .type = APP_EVENT_INIT,
    //     .data = NULL
    // };

    // if (app_step(app_ctx, &ev_init) != 0) {
    //     goto cleanup;
    // } else {
    //     ok_prompt("Initialization successful.");
    // }

    app_event_data_t ev_start = {
        .type = APP_EVENT_START,
        .data = NULL
    };

    if (app_step(app_ctx, &ev_start) != 0)
        goto cleanup;

    server_running(app_ctx);

cleanup:
    app_event_data_t ev_stop = {
        .type = APP_EVENT_STOP,
        .data = NULL
    };

    app_step(app_ctx, &ev_stop);
    app_context_destroy(app_ctx);
    return 0;
}

