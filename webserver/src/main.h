#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    char *banner;
} variables_t;

typedef struct {
    char *config_file;

    char *webserver_ip;
    int webserver_port;

    int router_mode;
    int router_port;

    int verbose;
    int debug;

    int dry_run;

    char *log_dir;
    int disable_logs;

    char *env;

} server_options_t;

typedef struct {
    server_options_t *options;
    variables_t *vars;
} app_context_t;

#endif
