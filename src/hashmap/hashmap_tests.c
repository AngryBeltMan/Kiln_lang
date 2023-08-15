#include "hashmap.h"
#include "hashmap.c"
#include <stdio.h>

uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const VarData *user = item;
    return hashmap_sip(user->name, strlen(user->name), seed0, seed1);
}

int user_compare(const void *a, const void *b, void *udata) {
    const VarData* str_a = a;
    const VarData* str_b = b;
    return strcmp(str_a->name,  str_b->name);
}

int main() {
    Hashmap* hmap = hashmap_new(sizeof(VarData), 8 * 64,0,0,user_hash, user_compare, NULL, NULL);
    char *name = malloc(4);
    char *type = malloc(8);
    strcpy(name, "joe");
    strcpy(type, "builder");
    hashmap_set(hmap, &(VarData) {.name = name, .type = type});
    VarData *data = (VarData*)hashmap_get(hmap, &(VarData) {.name = "jklfdjsaklfj"});
    if (data == NULL) {
        printf("NULL\n");
    }
    /* printf("val %s\n",data->type); */
    hashmap_free(hmap);

}
