#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HashMap HASHMAP_new() {
    HashMap hashmap;
    hashmap.list = malloc(sizeof(Index*) * HASHMAP_SIZE);
    for (int i = 0; i < HASHMAP_SIZE; ++i) {
        hashmap.list[i] = NULL;
    }
    return hashmap;
}
void HASHMAP_drop(HashMap hashmap) {
    for (int i = 0; i < HASHMAP_SIZE; ++i) {
        if (hashmap.list[i]) {
            printf("freeing %i\n",i);
            printf("freeing %s\n",hashmap.list[i]->name);
            free(hashmap.list[i]);
        }
    }
    free(hashmap.list);
}

void HASHMAP_insert(HashMap *P_hashmap,char* P_name, void* P_value) {
    uint total = 0;
    for (int i = 0; i < strlen(P_name); ++i) {
        total += (int)P_name[i];
    }
    uint hash = total % HASHMAP_SIZE;
    const uint original_hash = hash;
    int inserted = 0;
    do {
        if (!P_hashmap->list[hash]) {
            Index *index = malloc(sizeof(Index));
            index->name = P_name;
            index->value = P_value;
            P_hashmap->list[hash] = index;
            inserted = 1;
        } else {
            ++hash;
            if (hash > HASHMAP_SIZE) { hash = 0; }
            if (hash == original_hash) {printf("no more empty space\n"); break; }
        }
    } while (!inserted);
    printf("total %i\n",total);
    printf("hash %i\n",hash);
}
void* HASHMAP_get(HashMap*P_hashmap,char* P_name) {
    uint total = 0;
    for (int i = 0; i < strlen(P_name); ++i) {
        total += (int)P_name[i];
    }
    uint hash = total % HASHMAP_SIZE;
    const uint original_hash = hash;
    int aquired = 0;
    void* value = NULL;
    do {
        if (P_hashmap->list[hash]) {
            printf("name len %i\n",(int)strlen(P_hashmap->list[hash]->name));
            printf("idk len %i\n",(int)strlen(P_name));
            if (!strcmp(P_hashmap->list[hash]->name,P_name)) {
                printf("real");
                value = P_hashmap->list[hash]->value;
                aquired = 1;
            }
        } else {
            ++hash;
            if (hash > HASHMAP_SIZE) { hash = 0; }
            if (hash == original_hash) {printf("item doesn't exist"); break; }
        }
    } while (!aquired);
    return value;
}
