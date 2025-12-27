#ifndef INIT_H
#define INIT_H

// États possibles lors de l'initialisation
typedef enum {
    INIT_OK = 0,
    INIT_ASSETS_MISSING,
    INIT_ASSETS_NOT_DIR,
    INIT_CONFIG_ERROR
} init_status_t;


// Vérifications (logique pure)
init_status_t init_check_assets(void);
init_status_t init_check_config(void);

#endif /* INIT_H */
