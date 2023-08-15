#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../heap_array.h"
#include "../hashmap/hashmap.h"
#include "compiler_parser.h"
#include "../lib_tools.h"
#include "compiler_parser.c"
#include "modules.h"

Compiler COMPILER_new(char* output_file) {
    Compiler compiler;
    FILE *fs = fopen(output_file, "w");
    compiler.file = fs;
    compiler.contents = CONTENTS_new();
    compiler.modules = VECTOR_new();
    compiler.included_files = STRINGLIST_new();
    COMPILER_add_module(&compiler, MODULE_stdlib);
    return compiler;
}

void COMPILER_parse(Compiler *P_comp, Expressions *P_exprs, Hashmap *P_hashmap,Hashmap *P_func_map, char** P_module_name) {
    for (int i = 0; i < (P_exprs->size / sizeof(Expression)); ++i) {
        unsigned int token_index = 0;
        Token token = P_exprs->exprs[i].tokens[token_index];
        switch (token.token_type) {
            case TokenType_Ident: {
                parse_expression_from_keyword(get_ident_type(token.value), P_comp,P_exprs, i, P_hashmap, P_func_map, P_module_name);
                continue;
            }
            case TokenType_RightBracket: {
                CONTENTS_append_str(&P_comp->contents, "}\n");
                if (IS_PUBLIC) {
                    CONTENTS_append_str(&P_comp->contents, "#endif\n");
                    IS_PUBLIC = 0;
                }
                continue;
            }
            case TokenType_c_call: {
                Contents c_code =  token_parse_expression_to_end(&P_exprs->exprs[i], 1);
                CONTENTS_append_formatted(&P_comp->contents,"%s;\n", c_code.file);
                CONTENTS_drop(c_code);
                break;
            }
            case TokenType_AtSign: {
                IdentType keyword = SETTINGS_get_identtype(&P_exprs->exprs[i]);
                parse_expression_from_keyword( keyword, P_comp, P_exprs, i, P_hashmap,P_func_map, P_module_name);
                continue;
            }
            default: {
                continue;
            }
        }
    }
}

void COMPILER_write_to_file(Compiler *P_comp) {
    MODULES_write_to_file(P_comp);

    if (INITED_HEAP_ARRAY) { fprintf(P_comp->file, "%s", HEAPARRAYCONTENTS); }

    fprintf(P_comp->file, "%s", P_comp->contents.file);

    if (INITED_MAIN) fprintf(P_comp->file,"\nint main() {\n");

    if (INITED_HEAP_ARRAY) { fprintf(P_comp->file, "__HeapArray ___heap = __HeapArrayNew();\n"); }

    // adds the heap array argument to the main function if it INITED_HEAP_ARRAY is true
    if (INITED_MAIN) fprintf(P_comp->file,"__MAIN(%s);\n",INITED_HEAP_ARRAY ? "&___heap":" ");
    if (INITED_HEAP_ARRAY) { fprintf(P_comp->file, "__HeapArrayDrop(___heap);\n"); }
    if (INITED_MAIN) fprintf(P_comp->file, "return 0;\n }");
}

void COMPILER_drop(Compiler P_comp) {
#ifdef SHOW_CONTENTS
    printf("%s\n", P_comp.contents.file);
#endif
    VECTOR_drop(P_comp.modules);
    free(P_comp.contents.file);
}

