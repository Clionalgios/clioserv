#ifndef HTML_RENDERER_H
#define HTML_RENDERER_H

char *compose_page(const char *url,
                   const char *media,
                   const char *language,
                   const char *style_sheet);

static char *resolver(const char *key, const char *language);


#endif
