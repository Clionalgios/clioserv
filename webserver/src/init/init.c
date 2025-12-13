#include <stdio.h>

#define CLEAR_SCREEN "\x1B[2J\x1B[H"

void init(int argc, char *argv[]) {
    printf(CLEAR_SCREEN);
}