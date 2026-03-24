#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_BANNER_LENGTH 4096
#define MAX_LINE_LENGTH 1024

char *strreplace(char *s, const char *s1, const char *s2) {
    char *result;
    int i, cnt = 0;
    int newlen = strlen(s2);
    int oldlen = strlen(s1);

    // Compter le nombre d'occurrences de s1 dans s
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], s1) == &s[i]) {
            cnt++;
            i += oldlen - 1;
        }
    }

    // Allouer de la mémoire pour la nouvelle chaîne
    // Calculer la taille correcte pour la nouvelle chaîne
    size_t orig_len = strlen(s);
    size_t alloc_len = orig_len + cnt * (newlen > oldlen ? (newlen - oldlen) : (newlen - oldlen));
    result = (char *) malloc(alloc_len + 1);
    if (result == NULL) {
        return NULL; // Échec de l'allocation mémoire
    }

    i = 0;
    while (*s) {
        // Trouver la prochaine occurrence de s1
        if (strstr(s, s1) == s) {
            strcpy(&result[i], s2);
            i += newlen;
            s += oldlen;
        } else {
            result[i++] = *s++;
        }
    }

    result[i] = '\0';
    return result;
}



char* get_current_dir() {
    static char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return cwd;
    } else {
        perror("getcwd() error");
        return NULL;
    }
}

char* get_server_banner() {
    char* current_dir = get_current_dir();

    // Allouer suffisamment d'espace pour le chemin complet
    char* config_path = malloc(strlen(current_dir) + strlen("/settings.conf") + 1);
    if (!config_path) {
        perror("malloc failed");
        return NULL;
    }

    // Construire le chemin correctement
    strcpy(config_path, current_dir);
    strcat(config_path, "/settings.conf");

    FILE* file = fopen(config_path, "r");
    if (!file) {
        perror("Failed to open config file");
        free(config_path);
        return NULL;
    }

    free(config_path); // On n'a plus besoin du chemin après ouverture

    char* banner = malloc(MAX_BANNER_LENGTH);
    if (!banner) {
        fclose(file);
        return NULL;
    }
    banner[0] = '\0'; // Initialize empty string

    char line[MAX_LINE_LENGTH];
    int in_banner_section = 0;
    int banner_length = 0;
    int multi_line_banner = 0;

    while (fgets(line, sizeof(line), file)) {
        // Trim leading whitespace
        char* trimmed = line;
        while (isspace((unsigned char)*trimmed)) {
            trimmed++;
        }

        // Skip empty lines and comments
        if (*trimmed == '\0' || *trimmed == '#') {
            continue;
        }

        // Check for banner_as_banner option
        if (!in_banner_section && strncmp(trimmed, "banner_as_banner", 16) == 0) {
            char* equals = trimmed + 16;

            while (isspace((unsigned char)*equals)) equals++;

            if (*equals != '=') continue;

            equals++;
            while (isspace((unsigned char)*equals)) equals++;

            if (*equals != '"') continue;
            equals++;

            // Copy content after opening quote
            while (*equals && banner_length < MAX_BANNER_LENGTH - 1) {
                if (*equals == '\\' && *(equals + 1) == '"') {
                    banner[banner_length++] = '"';
                    equals += 2;
                } else if (*equals == '"') {
                    // Closing quote found
                    equals++;
                    in_banner_section = 1;
                    break;
                } else {
                    banner[banner_length++] = *equals++;
                }
            }
            banner[banner_length] = '\0';


            // If we didn't find the closing quote, we're in a multi-line banner
            if (!in_banner_section) {
                multi_line_banner = 1;
            }
            continue;
        }

        // Handle multi-line banner content
        if (multi_line_banner) {
            char* content_start = trimmed;
            char* quote_pos = strchr(content_start, '"');

            // If we find a closing quote on this line
            if (quote_pos) {
                // Copy content until the closing quote
                while (content_start < quote_pos && banner_length < MAX_BANNER_LENGTH - 1) {
                    if (*content_start == '\\' && *(content_start + 1) == '"') {
                        banner[banner_length++] = '"';
                        content_start += 2;
                    } else {
                        banner[banner_length++] = *content_start++;
                    }
                }
                banner[banner_length] = '\0';
                in_banner_section = 1;
                break;
            } else {
                // Copy the entire line
                while (*content_start && banner_length < MAX_BANNER_LENGTH - 1) {
                    if (*content_start == '\\' && *(content_start + 1) == '"') {
                        banner[banner_length++] = '"';
                        content_start += 2;
                    } else {
                        banner[banner_length++] = *content_start++;
                    }
                }
                
            }
            banner[banner_length] = '\0';
        }
    }

    fclose(file);

    if (!in_banner_section) {
        free(banner);
        return NULL;
    }

    return banner;
}

// Fonction utilitaire pour lire le contenu d'un fichier dans une chaîne allouée dynamiquement
char *read_file_content(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) return NULL;

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }
    long file_size = ftell(file);
    if (file_size < 0) {
        fclose(file);
        return NULL;
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }

    char *content = malloc(file_size + 1);
    if (content == NULL) {
        fclose(file);
        return NULL;
    }

    fread(content, 1, file_size, file);
    fclose(file);
    content[file_size] = '\0';
    return content;
}

void get_cookie_value(struct mg_http_message *hm, char *cookie_name, char **cookie_value) {
    struct mg_str *cookie_header = mg_http_get_header(hm, "Cookie");
    if (cookie_header == NULL) {
        printf("Untagged user on website\n");
        *cookie_value = NULL;
        return;
    }

    char *cookies = strndup(cookie_header->buf, cookie_header->len);
    if (cookies == NULL) {
        printf("Failed to allocate memory for cookies.\n");
        *cookie_value = NULL;
        return;
    } else {
        printf("Cookies found : \n");
        for (char *p = cookies; *p; p++) {
            if (*p == ';') {
                putchar('\n');
                while (*(p + 1) == ' ') p++; // Skip spaces after semicolon
                continue;
            }
            putchar(*p);
        }
        putchar('\n');
    }

    char *token = strtok(cookies, "; ");
    while (token != NULL) {
        char *eq_pos = strchr(token, '=');
        if (eq_pos != NULL) {
            size_t name_len = eq_pos - token;
            if (strncmp(token, cookie_name, name_len) == 0 && cookie_name[name_len] == '\0') {
                *cookie_value = strdup(eq_pos + 1);
                free(cookies);
                return;
            }
        }
        token = strtok(NULL, "; ");
    }

    free(cookies);
    *cookie_value = NULL;
}