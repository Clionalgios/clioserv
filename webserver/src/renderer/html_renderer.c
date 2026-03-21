#include "utils.h"
#include "html_renderer.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Remplace les placeholders de contenu dans la page avec les contenus appropriés stockés dans les fichiers json correspondants aux éléments HTML, identifiés par la langue
void fetch_content(const char **language, char **page) {
    int page_length = strlen(*page);
    for (int i = 0; i < page_length; i++) {
        if (strncmp(&(*page)[i], "{{{", 3) == 0) {
            char *end = strstr(&(*page)[i], "}}}");
            if (end) {
                size_t key_len = end - (&(*page)[i] + 3);
                char *key = strndup(&(*page)[i] + 3, key_len);
                if (key == NULL) {
                    return;
                }

                // Séparer element et content_name
                char *underscore_pos = strchr(key, '_');
                if (underscore_pos == NULL) {
                    printf("Invalid key format: %s\n", key);
                    free(key);
                    continue;
                }
                char *element = strndup(key, underscore_pos - key);
                char *content_name = strdup(key);
                if (!element || !content_name) {
                    free(key);
                    free(element);
                    free(content_name);
                    return;
                }

                // Charger le JSON correspondant
                char filename[512];
                snprintf(filename, sizeof(filename),
                         "./website_assets/content/%s.json", element);

                char *file_content = read_file_content(filename);
                if (!file_content) {
                    printf("Failed to read file: %s\n", filename);
                    free(key); free(element); free(content_name);
                    continue;
                }

                // Parser le JSON
                cJSON *json = cJSON_Parse(file_content);
                free(file_content);
                if (!json) {
                    printf("JSON parse error for %s\n", filename);
                    free(key); free(element); free(content_name);
                    continue;
                }

                // Trouver la clé (ex: "header_welcome_text")
                cJSON *entry = cJSON_GetObjectItemCaseSensitive(json, content_name);
                if (entry) {
                    // Récupérer la langue demandée
                    cJSON *lang_value = cJSON_GetObjectItemCaseSensitive(entry, *language);
                    if (cJSON_IsString(lang_value) && (lang_value->valuestring != NULL)) {
                        printf("Replacing key %s with value: %s\n", key, lang_value->valuestring);

                        // Construire la nouvelle page avec la valeur substituée
                        size_t before_len = (&(*page)[i]) - *page;
                        size_t after_len = strlen(end + 3);
                        size_t new_size = before_len + strlen(lang_value->valuestring) + after_len + 1;

                        char *new_page = malloc(new_size);
                        if (new_page) {
                            strncpy(new_page, *page, before_len);
                            new_page[before_len] = '\0';
                            strcat(new_page, lang_value->valuestring);
                            strcat(new_page, end + 3);

                            free(*page);
                            *page = new_page;

                            // Avancer l’index pour continuer après la valeur insérée
                            i = before_len + strlen(lang_value->valuestring) - 1;
                        }
                    }
                }

                cJSON_Delete(json);
                free(key);
                free(element);
                free(content_name);
            }
        }
    }
}


void fetch_element(const char *element, char **response) {
    char *filename = malloc(strlen("./website_assets/html/") + strlen(element) + 1);
    if (filename == NULL) {
        return;
    }
    sprintf(filename, "./website_assets/html/%s.html", element);

    char *content = read_file_content(filename);
    if (content == NULL) {
        free(filename);
        return;
    }

    char *placeholder = malloc(strlen(element) + 6);
    sprintf(placeholder, "{{{%s}}}", element);
    *response = strreplace(*response, placeholder, content);
    
    free(content);
    free(filename);
    free(placeholder);
}

void compose_overlay(char **response, const char **language) {
    const char *overlay_elements[4] = {
        "layout",
        "header",
        "footer"
    };

    // printf("Current state of response : %s\n", *response);

    for (int i = 0; i < 3; i++) {
        // printf("Fetching overlay element: %s\n", overlay_elements[i]);
        fetch_element(overlay_elements[i], response);
        // printf("Current state of response : %s\n", *response);
    }
    fetch_content(language, response);
}

// The returned string must be freed by the caller.
char *compose_page(const char *url, const char *media, const char *language, const char *style_sheet) { // à remettre dans le bloc webserver
    printf("%s", &media); // TODO la prise en compte du type d'appareil du client
    printf("%s", &style_sheet); // TODO la même pour le style
    char *response = "{{{body}}}";
    fetch_element("body", &response);

    struct templates *tpl = malloc(sizeof(struct templates));
    if (tpl == NULL) {
        return NULL;
    }

    compose_overlay(&response, &language);

    // OUVERTURE DU BLOC DE CONTENU
    if (strcmp(url, "/") == 0 || strcmp(url, "/favicon.ico") == 0) {
        url = "index.html";
    }
    printf("Mapped URI to file: %s\n", url);

    // Replace the "{{{main}}}" tag with the content of the file designated by the URL minus the .html extension
    char *filename = malloc(strlen("./website_assets/html/") + strlen(url) + 1);
    if (filename == NULL) {
        return NULL;
    }
    sprintf(filename, "./website_assets/html/%s", url);
    printf("Loading main template from file: %s\n", filename);
    tpl->main_template = read_file_content(filename);
    if (tpl->main_template == NULL) {
        free(tpl->main_template);
        tpl->main_template = read_file_content("./website_assets/html/404.html");
        printf("File not found, loading 404 template instead.\n");
    }
    printf("Main template content: \n%s\n", tpl->main_template ? tpl->main_template : "NULL");
    // FERMETURE DU BLOC DE CONTENU

    // OUVERTURE DU BLOC D'ASSEMBLAGE
    char *tmp;

    tmp = strreplace(response, "{{{main}}}", tpl->main_template);

    if (tpl->main_template != NULL) {
        free(tpl->main_template);
    }
    free(filename);
    free(tpl);
    // FERMETURE DU BLOC D'ASSEMBLAGE
    if (tmp == NULL) {
        free(response);
        return NULL;
    } else {
        response = tmp;
    }

    return response;
}