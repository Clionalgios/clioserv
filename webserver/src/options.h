#ifndef OPTIONS_H
#define OPTIONS_H

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

#endif
