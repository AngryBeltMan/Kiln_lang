#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "compiler.h"
#include "heap_array.h"
#include "contents.c"
#include "parser.h"
#define TOKEN_CMP(ident,name,type) if (!strcmp(ident,name)) { return type; }

static int INITED_HEAP_ARRAY = 0;

Compiler COMPILER_new() {
    Compiler compiler;
    FILE *fs = fopen(BUILDNAME, "w");
    compiler.file = fs;
    compiler.contents = CONTENTS_new();
    return compiler;
}
void expr(Expression e) {}

void init_heap_array() {

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

void write_print(Compiler* P_comp,Expression* P_expr,char* formatting) {
    CONTENTS_append_str(&P_comp->contents, "printf(\"");
    Contents contents = token_string_parse(P_expr, 3);
    CONTENTS_append_str(&P_comp->contents, contents.file);
    CONTENTS_append_str(&P_comp->contents, formatting);
    free(contents.file);
    contents.file = NULL;
}

IdentType get_ident_type(char *ident) {
    TOKEN_CMP(ident, "print",IdentType_print);
    TOKEN_CMP(ident, "println", IdentType_println);
    TOKEN_CMP(ident, "fprint",IdentType_printf);
    TOKEN_CMP(ident, "fprintln",IdentType_printf);
    TOKEN_CMP(ident, "let",IdentType_var_init);
    TOKEN_CMP(ident, "var",IdentType_varg_var);
    TOKEN_CMP(ident, "ref",IdentType_varg_var);
    TOKEN_CMP(&ident[0], "$",IdentType_vartype);
    return IdentType_varname;
}
void variable_value_parse(int ident_token,Expression* P_expr,Compiler* P_comp,char* var_type,char* var_name) {
    for (; ident_token < (P_expr->size)/sizeof(Token); ++ident_token) {
        Token value = P_expr->tokens[ident_token];
        switch (value.token_type) {
            case TokenType_DoubleQuote:
                printf("");
                Contents value_str = token_string_parse(P_expr, ident_token + 1);
                printf("value str %s\n",value_str.file);
                CONTENTS_append(&P_comp->contents, '"');
                CONTENTS_append_str(&P_comp->contents, value_str.file);
                CONTENTS_append(&P_comp->contents, '"');
                free(value_str.file);
                value_str.file = NULL;
                return;
            case TokenType_Ident:
                // TODO check if it is a function call
                if (!strcmp(value.value, "NONE")) { break; }
                if (!strcmp(value.value, "ref")) { CONTENTS_append(&P_comp->contents,'&'); continue;  }
                if (!strcmp(value.value, "@heap")) {
                    CONTENTS_append_str(&P_comp->contents, "malloc(sizeof(");
                    CONTENTS_append_str(&P_comp->contents, var_type);
                    CONTENTS_append_str(&P_comp->contents, "));\n");
                    if (!INITED_HEAP_ARRAY) {
                        INITED_HEAP_ARRAY = 1;
                        CONTENTS_append_str(&P_comp->contents,"__HeapArray ___heap = __HeapArrayNew();\n");
                    }
                    CONTENTS_append_str(&P_comp->contents,"__HeapArrayAppend(&___heap,(void*)");
                    CONTENTS_append_str(&P_comp->contents,var_name);
                    CONTENTS_append_str(&P_comp->contents,");\n");
                    CONTENTS_append(&P_comp->contents,'*');
                    CONTENTS_append_str(&P_comp->contents,var_name);
                    CONTENTS_append(&P_comp->contents,'=');
                    continue;
                }
                printf("var value %s\n",value.value);
                CONTENTS_append_str(&P_comp->contents,value.value);
                continue;
            default:
                continue;
        }
    }
}

void COMPILER_parse(Compiler *P_comp,Expressions *P_exprs) {
    fprintf(P_comp->file, "#include<stdio.h>\n#include<stdlib.h>\n");
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
                        printf("initing var\n");
                        Token name;
                        int ident_token = 2;
                        // get the name of the variable
                        for (; ident_token < (P_exprs->exprs[i].size)/sizeof(Token); ++ident_token) {
                            printf("index %i\n",ident_token);
                            if (P_exprs->exprs[i].tokens[ident_token].token_type == 0) {
                                name = P_exprs->exprs[i].tokens[ident_token];
                                break;
                            }
                        }
                        ++ident_token;
                        Token type;
                        // get the type of the variable
                        for (; ident_token < (P_exprs->exprs[i].size)/sizeof(Token); ++ident_token) {
                            printf("index a %i\n",ident_token);
                            if (P_exprs->exprs[i].tokens[ident_token].token_type == 7) {
                                type = P_exprs->exprs[i].tokens[ident_token + 1];
                                break;
                            }
                        }
                        ident_token += 2;
                        printf("index b %i\n",ident_token);
                        CONTENTS_append_str(&P_comp->contents,type.value);
                        CONTENTS_append_str(&P_comp->contents,name.value);
                        CONTENTS_append(&P_comp->contents,'=');
                        // get the value of the variable
                        variable_value_parse(ident_token, &P_exprs->exprs[i], P_comp,type.value,name.value);
                        CONTENTS_append_str(&P_comp->contents,";\n");
                        printf("%s %s\n",type.value,name.value);
                        continue;
                    default:
                        continue;
                }
            default:
                printf("other type %i\n",token.token_type);
                continue;
        }
    }
    CONTENTS_append_str(&P_comp->contents, "__HeapArrayDrop(___heap);\n");
    CONTENTS_append_str(&P_comp->contents,"return 0; \n }");
}
void COMPILER_write_to_file(Compiler* P_comp) {
    if (INITED_HEAP_ARRAY) {
        fprintf(P_comp->file, "%s",HEAPARRAYCONTENTS);
    }
    fprintf(P_comp->file, "%s",P_comp->contents.file);
}
void COMPILER_drop(Compiler P_comp) {
    printf("%s\n",P_comp.contents.file);
    free(P_comp.contents.file);
    fclose(P_comp.file);
}
