#ifndef PROMPTS_H
#define PROMPTS_H

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

void ok_prompt(const char *fmt, ...);
void info_prompt(const char *fmt, ...);
void error_prompt(const char *fmt, ...);
void prompt_set_logfile(FILE *fp);

#endif