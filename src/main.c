#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "contents.c"
#include "parser/parser.h"
#include "parser/parser.c"
#include "compiler/compiler.h"
#include "compiler/compiler.c"
#include "hashmap/hashmap.h"
#include "hashmap/hashmap.c"

uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const VarData *user = item;
    return hashmap_sip(user->name, strlen(user->name), seed0, seed1);
}
int user_compare(const void *a, const void *b, void *udata) {
    const VarData* str_a = a;
    const VarData* str_b = b;
    return strcmp(str_a->name,  str_b->name);
}

int main(void) {
    Hashmap* var_map = hashmap_new(sizeof(VarData), 8 * 64,0,0,user_hash, user_compare, NULL, NULL);
    FILE *main_file;
    main_file = fopen("main.kiln", "r");

    Expressions exprs =  EXPRESSIONS_from_file(main_file);

    Compiler comp = COMPILER_new();


    COMPILER_parse(&comp,&exprs, var_map);
    COMPILER_write_to_file(&comp);

    EXPRESSIONS_drop(exprs);
    COMPILER_drop(comp);

    fclose(main_file);
    hashmap_free(var_map);
    return 0;
}
