#include "utils.h"
#include "html_renderer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Remplace les placeholders de contenu dans la page avec les contenus appropriés stockés dans les fichiers json correspondants aux éléments HTML, identifiés par la langue
void fetch_content(const char **language, char **page) {
    // On remplace les placeholders de contenu en traversant la page avec une boucle
    for (int i = 0; i < strlen(*page); i++) {
        if (strncmp(&(*page)[i], "{{{", 3) == 0) {
            char *end = strstr(&(*page)[i], "}}}");
            if (end) {
                size_t key_len = end - (&(*page)[i] + 3);
                char *key = malloc(key_len + 1);
                if (key == NULL) {
                    return;
                }
                strncpy(key, &(*page)[i] + 3, key_len);
                key[key_len] = '\0';

                // Construire le chemin du fichier
                printf("Fetching content for key: %s\n", key);
                // Récupère la chaîne de caractères correspondant à l'élément récupéré (<element>_<content_name>)
                char *underscore_pos = strchr(key, '_');
                if (underscore_pos == NULL) {
                    printf("Invalid key format (no underscore): %s\n", key);
                    free(key);
                    continue;
                }
                // On récupère la suite de la chaîne après le premier underscore
                char *content_name = strdup(underscore_pos + 1);
                if (content_name == NULL) {
                    free(key);
                    return;
                }
                char *element = strndup(key, underscore_pos - key);
                if (element == NULL) {
                    free(key);
                    free(content_name);
                    return;
                }

                printf("Element: %s, Content Name: %s\n", element, content_name);
                
                char *filename = malloc(strlen("./webserver/src/website_assets/content/") + strlen(element) + 6);
                if (filename == NULL) {
                    printf("Memory allocation failed for filename\n");
                    free(key);
                    free(content_name);
                    return;
                }
                sprintf(filename, "./webserver/src/website_assets/content/%s.json", element);
                printf("Loading content from file: %s\n", filename);
                char *file_content = read_file_content(filename);
                if (file_content == NULL) {
                    printf("Failed to read file: %s\n", filename);
                    free(filename);
                    free(key);
                    free(content_name);
                    continue;
                }

                // On itère dans le fichier ligne par ligne
                for (char *line = strtok(file_content, "\n"); line != NULL; line = strtok(NULL, "\n")) {
                    // On cherche la ligne qui commence par la clé de contenu
                    char *search_key = malloc(strlen(content_name) + 4); // +4 pour les guillemets et le deux-points
                    if (strncmp(*line, "\"", 1) == 0) {
                        sprintf(search_key, "\"%s\":", content_name);
                    } else {
                        sprintf(search_key, "%s:", content_name);
                    }
                    printf("Searching for key in line: %s\n", search_key);
                }
                
            } else {
                // Pas de balise fermante trouvée, sortir de la boucle
                printf("No closing tag found for key: %s\n", &(*page)[i]);
                break;
            }
        } else {
            // Continuer la boucle
            continue;
        }
    }
}

void fetch_element(const char *element, char **response) {
    char *filename = malloc(strlen("./webserver/src/website_assets/html/") + strlen(element) + 1);
    if (filename == NULL) {
        return;
    }
    sprintf(filename, "./webserver/src/website_assets/html/%s.html", element);

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
    char *response = "{{{body}}}";
    fetch_element("body", &response);

    struct templates *tpl = malloc(sizeof(struct templates));
    if (tpl == NULL) {
        return NULL;
    }

    compose_overlay(&response, &language);

    // OUVERTURE DU BLOC DE CONTENU
    if (strcmp(url, "/") == 0) {
        url = "index.html";
    }
    printf("Mapped URI to file: %s\n", url);

    // Replace the "{{{main}}}" tag with the content of the file designated by the URL minus the .html extension
    char *filename = malloc(strlen("./webserver/src/website_assets/html/") + strlen(url) + 1);
    if (filename == NULL) {
        return NULL;
    }
    sprintf(filename, "./webserver/src/website_assets/html/%s", url);
    printf("Loading main template from file: %s\n", filename);
    tpl->main_template = read_file_content(filename);
    if (tpl->main_template == NULL) {
        free(tpl->main_template);
        tpl->main_template = read_file_content("./webserver/src/website_assets/html/404.html");
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