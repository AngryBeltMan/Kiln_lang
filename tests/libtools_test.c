#include "../src/lib_tools.h"
#include "../src/lib_tools.c"
#include <string.h>

int main() {
    Vector vec = VECTOR_new();
    char* text = malloc(6);
    memmove(text, "hello", 6);
    VECTOR_append(&vec, text);
    char* cool = malloc(6);
    memmove(text, "world", 6);
    VECTOR_append(&vec, cool);
    char* wow = malloc(6);
    memmove(text, "hejkel", 6);
    VECTOR_append(&vec, wow);
    VECTOR_drop(vec);
    return 0;
}
