#include <stdio.h>
#include <string.h>
#include "hello.h"

int main(void) {
    const char* s = say_hello();
    if (s == NULL) {
        fprintf(stderr, "say_hello returned NULL\n");
        return 1;
    }
    if (strcmp(s, "Hello, World!") != 0) {
        fprintf(stderr, "Unexpected string: %s\n", s);
        return 1;
    }
    puts("Test passed");
    return 0;
}
