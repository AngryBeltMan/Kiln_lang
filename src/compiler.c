#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "compiler.h"
#include "heap_array.h"
#include "lib_tools.h"
#include "parser.h"

#include "lib_tools.c"
#include "contents.c"

#include "variables/variables.h"
#include "variables/variables.c"
#include "if_statements/if_statement.h"
#include "if_statements/if_statement.c"
#include "for_loop/forloop.h"
#include "for_loop/forloop.c"

#define TOKEN_CMP(ident,name,type) if (!strcmp(ident,name)) { return type; }

void write_print(Compiler* P_comp,Expression* P_expr,char* formatting) {
    COMPILER_add_module(P_comp, MODULE_stdio);
    Contents contents = token_string_parse(P_expr, 3);
    CONTENTS_append_formated(&P_comp->contents,"printf(\"%s%s",contents.file,formatting);
    free(contents.file);
    contents.file = NULL;
}

void COMPILER_add_module(Compiler *P_comp, Module module) {
    int *mod = malloc(sizeof(int));
    *mod = module;
    VECTORappend(&P_comp->modules,mod);
}

Compiler COMPILER_new() {
    Compiler compiler;
    FILE *fs = fopen(BUILDNAME, "w");
    compiler.file = fs;
    compiler.contents = CONTENTS_new();
    compiler.modules = VECTOR_new();
    COMPILER_add_module(&compiler, MODULE_stdlib);
    return compiler;
}

void COMPILER_parse(Compiler *P_comp,Expressions *P_exprs) {
    CONTENTS_append_str(&P_comp->contents,"\nint main(void) {\n");
    for (int i = 0; i < (P_exprs->size/sizeof(Expression)); ++i) {
        uint e = P_exprs->exprs[i].size;
        Token token = P_exprs->exprs[i].tokens[0];
        switch (token.token_type) {
            case TokenType_Ident:
                switch (get_ident_type(token.value)) {
                    // println("hello world");
                    case IdentType_println:
                        write_print(P_comp, &P_exprs->exprs[i], "\\n\");\n");
                        continue;
                    case IdentType_print:
                        write_print(P_comp, &P_exprs->exprs[i], "\");\n");
                        continue;
                        // let num $int = 10;
                    case IdentType_var_init:
                        printf("");
                        VarOpts varopts = VAROPTS_expression_parse(&P_exprs->exprs[i], P_comp);
                        VAROPTS_create_var(varopts, P_comp);
                        continue;
                    case IdentType_if_statement:
                        printf("");
                        IfStatementOpt ifopts = IFSTATEMENT_parse(&P_exprs->exprs[i]);
                        IFSTATEMENT_write_to_file(P_comp, ifopts);
                        continue;
                    case IdentType_for_loop:
                        printf("");
                        ForLoopOpt for_opts = FORLOOP_parse(&P_exprs->exprs[i]);
                        FORLOOP_write_to_file(P_comp, for_opts);
                        continue;
                    default:
                        printf("other type %i\n",token.token_type);
                        continue;
                }
            case TokenType_RightBracket:
                CONTENTS_append_str(&P_comp->contents, "}\n");
                continue;
            case TokenType_RightArrow:
                printf("arrow\n");
                continue;
            default:
                continue;
        }
    }
    if (INITED_HEAP_ARRAY) { CONTENTS_append_str(&P_comp->contents, "__HeapArrayDrop(___heap);\n"); }

    CONTENTS_append_str(&P_comp->contents,"return 0; \n }");
}

void COMPILER_write_to_file(Compiler* P_comp) {
    for (int module = 0; module < VECTOR_len(&P_comp->modules); ++module) {
        switch (*(int*)VECTOR_index(&P_comp->modules, module)) {
            case MODULE_stdlib:
                fprintf(P_comp->file,"#include<stdlib.h>\n");
                continue;
            case MODULE_stdio:
                fprintf(P_comp->file,"#include<stdio.h>\n");
                continue;
            case MODULE_string:
                fprintf(P_comp->file,"#include<string.h>\n");
                continue;
        }
    }
    if (INITED_HEAP_ARRAY) { fprintf(P_comp->file, "%s",HEAPARRAYCONTENTS); }
    fprintf(P_comp->file, "%s",P_comp->contents.file);
}

void COMPILER_drop(Compiler P_comp) {
    printf("%s\n",P_comp.contents.file);
    VECTOR_drop(P_comp.modules);
    free(P_comp.contents.file);
    fclose(P_comp.file);
}

Contents token_string_parse(Expression* P_expr,int start) {
    Contents string = CONTENTS_new();
    for (int text = start; text < (P_expr->size/sizeof(Token)); ++text) {
        Token *token = &P_expr->tokens[text];
        if (token->token_type == 5) { break; }
        if (token->value) {
            CONTENTS_append_str(&string,token->value);
        } else {
            CONTENTS_append(&string,token->character);
        }
    }
    return string;
}

IdentType get_ident_type(char *ident) {
    TOKEN_CMP(ident, "print",IdentType_print);
    TOKEN_CMP(ident, "println", IdentType_println);
    TOKEN_CMP(ident, "fprint",IdentType_printf);
    TOKEN_CMP(ident, "fprintln",IdentType_printf);
    TOKEN_CMP(ident, "let",IdentType_var_init);
    TOKEN_CMP(ident, "if",IdentType_if_statement);
    TOKEN_CMP(ident, "for",IdentType_for_loop);
    TOKEN_CMP(&ident[0], "}",IdentType_break_bracket);
    return IdentType_var_name;
}

