#include "html_renderer.h"
#include "utils.h"
#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LEN 256

// =========================
// 🔧 UTIL
// =========================

static char *read_file_safe(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = malloc(size + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, size, f);
    buf[size] = '\0';

    fclose(f);
    return buf;
}

// =========================
// 🔧 HTML RESOLVER
// =========================

static char *resolve_html(const char *key) {
    char path[512];
    snprintf(path, sizeof(path), "./website_assets/html/%s.html", key);

    char *content = read_file_safe(path);
    if (content) {
        printf("[HTML] %s\n", key);
    }
    return content;
}

// =========================
// 🔧 JSON RESOLVER (par fichier)
// =========================

static char *resolve_json(const char *key, const char *language) {
    // extraire prefix (avant '_')
    char element[128];
    const char *underscore = strchr(key, '_');

    if (!underscore) return NULL;

    size_t len = underscore - key;
    if (len >= sizeof(element)) return NULL;

    memcpy(element, key, len);
    element[len] = '\0';

    char path[512];
    snprintf(path, sizeof(path),
             "./website_assets/content/%s.json", element);

    char *file = read_file_safe(path);
    if (!file) return NULL;

    cJSON *json = cJSON_Parse(file);
    free(file);

    if (!json) return NULL;

    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);

    char *result = NULL;

    if (cJSON_IsObject(item)) {
        // ✅ cas multilangue
        cJSON *lang_node = cJSON_GetObjectItemCaseSensitive(item, language);

        if (cJSON_IsString(lang_node) && lang_node->valuestring) {
            result = strdup(lang_node->valuestring);
            printf("[JSON:%s] %s\n", language, key);
        } else {
            // fallback FR
            cJSON *fallback = cJSON_GetObjectItemCaseSensitive(item, "fr");

            if (cJSON_IsString(fallback) && fallback->valuestring) {
                result = strdup(fallback->valuestring);
                printf("[JSON:fallback-fr] %s\n", key);
            }
        }
    }
    else if (cJSON_IsString(item) && item->valuestring) {
        // ✅ cas simple (legacy)
        result = strdup(item->valuestring);
        printf("[JSON] %s\n", key);
    }

    cJSON_Delete(json);
    return result;
}

// =========================
// 🔧 GLOBAL RESOLVER
// =========================

static char *resolver(const char *key, const char *language) {
    char *v;

    v = resolve_html(key);
    if (v) return v;

    v = resolve_json(key, language);
    if (v) return v;

    printf("[MISS] %s\n", key);
    return strdup("");
}


// =========================
// 🔧 RENDER CORE
// =========================

static char *render(const char *template, const char *language){
    if (!template) return NULL;

    size_t cap = strlen(template) + 1;
    char *result = malloc(cap);
    if (!result) return NULL;

    result[0] = '\0';

    const char *p = template;

    while (*p) {
        const char *start = strstr(p, "{{{");

        if (!start) {
            strcat(result, p);
            break;
        }

        strncat(result, p, start - p);

        const char *end = strstr(start, "}}}");
        if (!end) {
            strcat(result, start);
            break;
        }

        size_t key_len = end - (start + 3);

        char key[MAX_KEY_LEN];
        if (key_len >= MAX_KEY_LEN)
            key_len = MAX_KEY_LEN - 1;

        memcpy(key, start + 3, key_len);
        key[key_len] = '\0';

        char *value = resolver(key, language);


        if (value) {
            size_t needed =
                strlen(result) +
                strlen(value) +
                strlen(end + 3) + 1;

            if (needed > cap) {
                cap = needed * 2;
                result = realloc(result, cap);
                if (!result) {
                    free(value);
                    return NULL;
                }
            }

            strcat(result, value);
            free(value);
        }

        p = end + 3;
    }

    return result;
}

// =========================
// 🚀 COMPOSE PAGE
// =========================

char *compose_page(const char *url,
                   const char *media,
                   const char *language,
                   const char *style_sheet) {

    (void)media;
    (void)language;
    (void)style_sheet;

    // =========================
    // 1. Charger layout principal
    // =========================

    char *page = read_file_safe("./website_assets/html/body.html");
    if (!page) return NULL;

    // =========================
    // 2. Charger contenu main
    // =========================

    char path[512];

    char clean_url[256];

    snprintf(clean_url, sizeof(clean_url), "%.*s",
            (int)strlen(url), url);

    // enlever trailing slash
    size_t len = strlen(clean_url);
    if (len > 1 && clean_url[len - 1] == '/')
        clean_url[len - 1] = '\0';

    // cas root
    if (strcmp(clean_url, "/") == 0) {
        snprintf(path, sizeof(path),
                "./website_assets/html/mains/index.html");
    } else {
        snprintf(path, sizeof(path),
                "./website_assets/html/mains%s.html", clean_url);
    }



    char *main_content = read_file_safe(path);

    if (!main_content) {
        printf("Fallback to 404 for %s\n", path);
        main_content = read_file_safe("./website_assets/html/404.html");
        if (!main_content) {
            free(page);
            return NULL;
        }
    }

    // =========================
    // 3. Injecter {{{main}}}
    // =========================

    char *tmp;

    // remplacer {{{main}}} à la main (simple et safe)
    char *placeholder = "{{{main}}}";
    char *pos = strstr(page, placeholder);

    if (pos) {
        size_t before_len = pos - page;
        size_t after_len = strlen(pos + strlen(placeholder));

        size_t new_size = before_len + strlen(main_content) + after_len + 1;

        tmp = malloc(new_size);
        if (!tmp) {
            free(page);
            free(main_content);
            return NULL;
        }

        memcpy(tmp, page, before_len);
        memcpy(tmp + before_len, main_content, strlen(main_content));
        memcpy(tmp + before_len + strlen(main_content),
               pos + strlen(placeholder),
               after_len);

        tmp[new_size - 1] = '\0';

        free(page);
        page = tmp;
    }

    free(main_content);

    // =========================
    // 4. Résolution récursive
    // =========================

    for (int i = 0; i < 3; i++) {
        tmp = render(page, language);
        free(page);
        page = tmp;
    }

    // printf("Final response:\n%s\n", page);

    return page;
}
