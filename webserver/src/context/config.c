#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

app_config_t *app_config_create(void) {
    app_config_t *app_config = calloc(1, sizeof(app_config_t));
    if (!app_config) return NULL;
    return app_config;
}

void app_config_destroy(app_config_t *cfg) {
    if (!cfg) return;
    free(cfg);
}

int app_config_set_file(app_config_t *config, const char *path)
{
    fprintf(stderr, "app_config_set_file: config=%p path=%p\n",
            (void *) config, (void *) path);

    if (!config) {
        return -1;
    }

    fprintf(stderr, "config_file address=%p\n",
            (void *) &config->config_file);

    free(config->config_file);
    config->config_file = NULL;

    if (!path) {
        return 0;
    }

    config->config_file = strdup(path);
    return config->config_file ? 0 : -1;
}

int app_config_set_verbosity(app_config_t *cfg, int v) {
    if (!cfg) return -1;

    cfg->verbosity = v;
    return 0;
}

const char *app_config_get_file(app_config_t *cfg)
{
    return cfg ? cfg->config_file : NULL;
}

int app_config_get_verbose(app_config_t *cfg)
{
    return cfg ? cfg->verbosity : 0;
}