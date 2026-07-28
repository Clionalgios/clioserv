#ifndef CONFIG_H
#define CONFIG_H

typedef struct app_config {
    char *config_file;
    int verbosity;
} app_config_t;

app_config_t *app_config_create(void);
void app_config_destroy(app_config_t *cfg);

int app_config_set_file(app_config_t *cfg, const char *path);
int app_config_set_verbosity(app_config_t *cfg, int v);

const char *app_config_get_file(app_config_t *cfg);
int app_config_get_verbose(app_config_t *cfg);

#endif