#ifndef RUNTIME_DATA_H
#define RUNTIME_DATA_H

typedef struct runtime_data {
    char *banner;
    char *webserver_ip;
    char *webserver_port;
    char *env;
} runtime_data_t;

runtime_data_t *runtime_data_create(void);
void runtime_data_destroy(runtime_data_t *rt);

int runtime_set_ip(runtime_data_t *rt, const char *ip);
int runtime_set_port(runtime_data_t *rt, const char *port);
int runtime_set_env(runtime_data_t *rt, const char *env);

const char *runtime_get_ip(runtime_data_t *rt);

#endif RUNTIME_DATA_H