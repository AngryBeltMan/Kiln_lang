#include <stdlib.h>

#ifndef LIBTOOLS
#define LIBTOOLS

int str_to_int(char* string);

typedef struct Vector {
    uint index;
    uint max;
    void** items;
} Vector;

Vector VECTOR_new();
Vector VECTOR_new_sized(uint size);
void VECTORappend(Vector *P_vector, void* P_item);
void* VECTOR_index(Vector *P_vector, uint index);
void VECTOR_drop(Vector vector);
uint VECTOR_len(Vector *P_vector);

#endif
