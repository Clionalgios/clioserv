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



char* get_server_banner(char* config_path) {
    if (config_path == NULL) {
        config_path = "settings.conf"; // Chemin par défaut
    }
    printf("%s", config_path);
    fprintf(stderr, "Hiiiii\n");
    FILE* file = fopen(config_path, "r");
    if (!file) {
        perror("Failed to open config file");
        return NULL;
    } else {
        printf("DEBUG file found");
    }

    char* banner = malloc(MAX_BANNER_LENGTH);
    if (!banner) {
        fclose(file);
        return NULL;
    }
    banner[0] = '\0'; // Initialize empty string

    char line[MAX_LINE_LENGTH];
    int in_banner_section = 0;
    int banner_length = 0;

    while (fgets(line, sizeof(line), file)) {
        // Trim leading whitespace
        char* trimmed = line;
        while (isspace((unsigned char)*trimmed)) {
            trimmed++;
        }

        // Check for banner_as_banner option
        if (strncmp(trimmed, "banner_as_banner", 16) == 0) {
            char* equals = strchr(trimmed, '=');
            if (!equals) {
                continue;
            }

            // Skip past equals sign and whitespace
            equals++;
            while (isspace((unsigned char)*equals)) {
                equals++;
            }

            // Check for opening quote
            if (*equals != '"') {
                continue;
            }
            equals++; // Skip opening quote

            // Copy everything until closing quote
            while (*equals && *equals != '"' && banner_length < MAX_BANNER_LENGTH - 1) {
                // Handle escaped quotes if needed
                if (*equals == '\\' && *(equals + 1) == '"') {
                    banner[banner_length++] = '"';
                    equals += 2;
                } else {
                    banner[banner_length++] = *equals++;
                }
            }
            banner[banner_length] = '\0';
            in_banner_section = 1;
            break;
        } else {
            printf("banner as banner not found");
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