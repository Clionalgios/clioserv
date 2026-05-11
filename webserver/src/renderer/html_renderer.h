#ifndef HTML_RENDERER_H
#define HTML_RENDERER_H

struct overlay_templates {
    char *layout_template;
    char *header_template;
    char *footer_template;
};

struct templates {
    struct overlay_templates *overlay;
    char *main_template;
};

struct response {
    char *body;
    struct templates *tpl;
};

char *compose_page(char *url, char *media, char *language, char *style_sheet);


#endif