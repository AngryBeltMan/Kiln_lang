#include "lib_tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#ifndef LIBTOOLSDEF
#define LIBTOOLSDEF
#define VECTOR_append(vec,item) printf("calling on line %i in file %s\n",__LINE__,__FILE__); VECTORappend(vec,item);

int str_to_int(char *string) {
    int value = 0;
    for (int pow = 0; pow < strlen(string); ++pow) {
        value += (int)string[pow]-48;
        value *= 10;
    }
    return value/10;
}

Vector VECTOR_new() {
    return VECTOR_new_sized(8);
}
Vector VECTOR_new_sized(uint size) {
    Vector vector;
    vector.index = 0;
    vector.max = size;
    vector.items = malloc(size);
    if (!vector.items) { printf("ERROR: could not malloc\n"); }
    return vector;

}
void VECTORappend(Vector* vector, void *P_item) {
    if (vector->index + 8 >= vector->max) {
        vector->max *= 2;
        void** array = realloc(vector->items, vector->max);
        if (!array) { printf("ERROR: failed to reallocate vector\n"); }
        vector->items = array;
    }
    printf("appending on index %i\n",(int)(vector->index/sizeof(void*)));
    vector->items[vector->index/sizeof(void*)] = P_item;
    vector->index += 8;
}
void VECTORappendint(Vector* vector, int *P_item) {
    printf("wha\n");
    if (vector->index * sizeof(void*) + sizeof(void*) >= vector->max) {
        vector->max *= 2;
        void** array = realloc(vector->items, vector->max + 8);
        if (!array) { printf("ERROR: failed to reallocate vector\n"); }
        vector->items = array;
    }
    printf("appending on index %i\n",(int)(vector->index/sizeof(void*)));
    vector->items[vector->index/sizeof(void*)] = P_item;
    vector->index += sizeof(void*);
}

void VECTOR_drop(Vector vector) {
    printf("dropping\n");
    for (int i = 0; i < (vector.index/sizeof(void*)); ++i) {
        printf("%i\n",*(int*)vector.items[i]);
        free(vector.items[i]);
        vector.items[i] = NULL;
    }
    free(vector.items);
}

void* VECTOR_index(Vector *P_vector,uint index) {
    if ((P_vector->index/sizeof(void*)) >= index) {
        return P_vector->items[index];
    }
    return NULL;
}
uint VECTOR_len(Vector *P_vector) {
    return (P_vector->index/sizeof(void*));
}
#endif
