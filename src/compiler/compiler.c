#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../heap_array.h"
#include "compiler_parser.h"
#include "compiler_parser.c"
#include "modules.h"


Compiler COMPILER_new() {
    Compiler compiler;
    FILE *fs = fopen(BUILDNAME, "w");
    compiler.file = fs;
    compiler.contents = CONTENTS_new();
    compiler.modules = VECTOR_new();
    COMPILER_add_module(&compiler, MODULE_stdlib);
    return compiler;
}



void COMPILER_parse(Compiler *P_comp, Expressions *P_exprs) {
    for (int i = 0; i < (P_exprs->size / sizeof(Expression)); ++i) {
        uint e = P_exprs->exprs[i].size;
        uint token_index = 0;
        Token token = P_exprs->exprs[i].tokens[token_index];
        switch (token.token_type) {
            case TokenType_Ident: {
                parse_expression_from_keyword(get_ident_type(token.value), P_comp,P_exprs, i);
                continue;
            }
            case TokenType_RightBracket: {
                CONTENTS_append_str(&P_comp->contents, "}\n");
                continue;
            }
            case TokenType_RightArrow: {
                continue;
            }
            case TokenType_AtSign: {
                IdentType keyword = SETTINGS_get_identtype(P_exprs->exprs);
                parse_expression_from_keyword( keyword, P_comp, P_exprs, i);
                continue;
            }
            default: {
                continue;
            }
        }
    }
}

void COMPILER_write_to_file(Compiler *P_comp) {
    if (INITED_HEAP_ARRAY) { fprintf(P_comp->file, "%s", HEAPARRAYCONTENTS); }
    MODULES_write_to_file(P_comp);

    fprintf(P_comp->file, "%s", P_comp->contents.file);

    fprintf(P_comp->file,"\nint main() {\n");

    if (INITED_HEAP_ARRAY) { fprintf(P_comp->file, "__HeapArray ___heap = __HeapArrayNew();\n"); }

    // adds the heap array argument to the main function if it INITED_HEAP_ARRAY is true
    fprintf(P_comp->file,"__MAIN(%s);\n",INITED_HEAP_ARRAY ? "&___heap":" ");
    if (INITED_HEAP_ARRAY) { fprintf(P_comp->file, "__HeapArrayDrop(___heap);\n"); }
    fprintf(P_comp->file, "return 0;\n }");
}

void COMPILER_drop(Compiler P_comp) {
    printf("%s\n", P_comp.contents.file);
    VECTOR_drop(P_comp.modules);
    free(P_comp.contents.file);
    fclose(P_comp.file);
}

