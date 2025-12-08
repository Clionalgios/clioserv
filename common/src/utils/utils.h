// #ifndef MONGOOSE_H
// #define MONGOOSE_H
// #include "mongoose.h"
// #endif



#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"

char *strreplace(char *s, const char *s1, const char *s2);
char *read_file_content(const char *filepath);

void get_cookie_value(struct mg_http_message *hm, char *cookie_name, char **cookie_value);

#endif