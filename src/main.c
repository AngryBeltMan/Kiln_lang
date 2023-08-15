#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "contents.c"
#include "functions/functions.h"
#include "parser/parser.h"
#include "parser/parser.c"
#include "compiler/compiler.h"
#include "compiler/compiler.c"
#include "hashmap/hashmap.h"
#include "hashmap/hashmap.c"
#include "cli/cli_tools.h"
#include "cli/cli_tools.c"

uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const VarData *user = item;
    return hashmap_sip(user->name, strlen(user->name), seed0, seed1);
}
int user_compare(const void *a, const void *b, void *udata) {
    const VarData* str_a = a;
    const VarData* str_b = b;
    return strcmp(str_a->name,  str_b->name);
}
uint64_t func_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const FuncMap *user = item;
    return hashmap_sip(user->function, strlen(user->function), seed0, seed1);
}
int func_compare(const void *a, const void *b, void *udata) {
    const FuncMap* str_a = a;
    const FuncMap* str_b = b;
    return strcmp(str_a->function,  str_b->function);
}

int main(int argc, char **argv) {
    char* module_name = "main";
    S_cli_settings = CLISETTINGS_identify_cli_type(argc,argv);
    Hashmap* var_map = hashmap_new(sizeof(VarData), 8 * 64,0,0,user_hash, user_compare, NULL, NULL);
    Hashmap* function_map = hashmap_new(sizeof(VarData), 8 * 64,0,0,func_hash, func_compare, NULL, NULL);
    FILE *main_file;
    main_file = fopen(S_cli_settings.file_name, "r");

    Expressions exprs =  EXPRESSIONS_from_file(main_file);

    Compiler comp = COMPILER_new("build.c");

    if (S_cli_settings.type == CLI_Translate) {
        fclose(comp.file);
        comp.file = fopen(S_cli_settings.info.Translate.output_name,"w");
    }

    COMPILER_parse(&comp,&exprs, var_map,function_map, &module_name);
    COMPILER_write_to_file(&comp);

    COMPILER_drop(comp);
    EXPRESSIONS_drop(exprs);

    fclose(main_file);
    hashmap_free(var_map);
    hashmap_free(function_map);

    KILNMODULES_translate(comp.included_files);

    CLISETTINGS_parse(&S_cli_settings);
    return 0;
}
