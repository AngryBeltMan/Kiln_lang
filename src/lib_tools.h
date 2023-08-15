#include <stdlib.h>
#ifndef LIBTOOLS
#define LIBTOOLS

static int IS_PUBLIC = 0;

int str_to_int(char* string);

typedef struct Vector {
    uint index;
    uint max;
    void** items;
} Vector;

typedef struct {
    unsigned int index;
    unsigned int max;
    char** items;
} StringList;

static inline StringList STRINGLIST_new() {
    return (StringList) {
        .index = 0,
        .max = 8,
        .items = malloc(sizeof(char*) * 8)
    };
}
static inline void STRINGLIST_append(StringList *string_list, char* item) {
    if (string_list->index + 1 > string_list->max) {
        string_list->max *= 2;
        string_list->items = realloc(string_list->items, string_list->max * sizeof(char*));
    }
    string_list->items[string_list->index] = item;
    string_list->index += 1;
}
Vector VECTOR_new();
Vector VECTOR_new_sized(unsigned int size);
void VECTORappend(Vector *P_vector, void* P_item);
void* VECTOR_index(Vector *P_vector, unsigned int index);
void VECTOR_drop(Vector vector);
unsigned int VECTOR_len(Vector *P_vector);

#endif
