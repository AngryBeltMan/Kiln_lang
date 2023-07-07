#include "../compiler.h"
#include "../lib_tools.h"
#include "../contents.c"
#include "variables.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
VarOpts VAROPTS_new() {
    VarOpts varopts;
    varopts.value = NULL;
    varopts.name = NULL;
    varopts.type = NULL;
    varopts.size_mult = 1;
    varopts.heap_allocated = 0;
    varopts.string = 0;
    return varopts;
}
void VAROPTS_create_var(VarOpts varopts,Compiler *P_comp) {
    if (varopts.heap_allocated) {
        Contents type = CONTENTS_from_char_slice_range(varopts.type,0,strlen(varopts.type)-2);
        CONTENTS_append_formated(&P_comp->contents,"%s %s = malloc(sizeof(%s) * %i);\n",varopts.type,varopts.name,type.file,varopts.size_mult);
        free(type.file);
        CONTENTS_append_formated(&P_comp->contents, "__HeapArrayAppend(&___heap, %s);\n",varopts.name);
        if (varopts.string) {
            COMPILER_add_module(P_comp, MODULE_string);
            printf("varopt value %s\n",varopts.value);
            CONTENTS_append_formated(&P_comp->contents, "strcpy(%s,%s);\n",varopts.name,varopts.value);
        } else {
            CONTENTS_append_formated(&P_comp->contents, "*%s = %s;\n",varopts.name,varopts.value);
        }
    } else {
        CONTENTS_append_formated(&P_comp->contents,"%s %s = %s;\n",varopts.type,varopts.name,varopts.value);
    }
    if (varopts.value != NULL) { free(varopts.value); }
}

char* get_var_ident(Expression* P_expr, int* index) {
    while (*index < (P_expr->size/sizeof(Token))) {
        if (P_expr->tokens[*index].token_type == TokenType_Ident) {
            return P_expr->tokens[*index].value;
        }
        *index += 1;
    }
    printf("ERROR: failed to get variable ident %s\n",__FUNCTION__);
    return NULL;
}

// let <name> $<type> = !<@args> <value>
// will stay the same right now will likely change
VarOpts VAROPTS_expression_parse(Expression *P_expr, Compiler *P_comp) {
    VarOpts varopts = VAROPTS_new();
    // skips over the let expression
    int expression_index = 1;
    varopts.name = get_var_ident(P_expr, &expression_index);
    if (varopts.name == NULL) { abort(); }
    expression_index += 1;
    varopts.type = get_var_ident(P_expr, &expression_index);
    if (varopts.type == NULL) { abort(); }
    expression_index += 1;
    variable_value_parse(&expression_index, P_expr, P_comp, &varopts);
    return varopts;
}

char* parse_setting_var(Expression *P_expr, int *varname_index) {
    int length = *varname_index + 2;
    int equal_sign_index = EXPRESSION_token_exist(P_expr, length, TokenType_EqualSign);
    if (equal_sign_index != -1) {
        // adding one to skip the ident token index
        *varname_index = equal_sign_index + 1;
        return P_expr->tokens[equal_sign_index + 1].value;
    }
    return NULL;
}

void variable_value_parse(int* ident_token,Expression* P_expr,Compiler* P_comp, VarOpts* varopts) {
    Contents var_value = CONTENTS_new();
    for (; *ident_token < (P_expr->size)/sizeof(Token); ++*ident_token) {
        Token value = P_expr->tokens[*ident_token];
        switch (value.token_type) {
            case TokenType_DoubleQuote:
                printf("");
                Contents str = token_string_parse(P_expr, *ident_token + 1);
                varopts->string = 1;
                CONTENTS_append_formated(&var_value, "\"%s\"",str);
                varopts->value = var_value.file;
                free(str.file);
                return;
            case TokenType_AtSign:
                printf("index %i\n",*ident_token);
                printf("setting var name %s\n",P_expr->tokens[*ident_token+1].value);
                printf("setting var value %s\n",P_expr->tokens[*ident_token+2].value);
                if (!strcmp(P_expr->tokens[*ident_token+1].value, "heap")) {
                    varopts->heap_allocated = 1;
                    if (!INITED_HEAP_ARRAY) {
                        INITED_HEAP_ARRAY = 1;
                        CONTENTS_append_str(&P_comp->contents,"__HeapArray ___heap = __HeapArrayNew();\n");
                    }
                    /* CONTENTS_append_formated(&P_comp->contents, "*%s=",var_name); */

                } else if (!strcmp(P_expr->tokens[*ident_token+1].value, "size")) {
                    char* setting_value = parse_setting_var(P_expr, ident_token);
                    assert(setting_value != NULL && "ERROR: could not parse @size");
                    varopts->size_mult = str_to_int(setting_value);
                }
                continue;
            case TokenType_Ident:
                // TODO check if it is a function call
                if (!strcmp(value.value, "NONE")) {
                    free(var_value.file);
                    var_value.file = NULL;
                    break;
                }
                if (!strcmp(value.value, "ref")) { CONTENTS_append(&var_value,'&'); continue;  }
                if (!strcmp(value.value, "heap")) { continue; }
                if (!strcmp(value.value, "size")) { continue; }
                CONTENTS_append_str(&var_value,value.value);
                continue;
            default:
                printf("other %i\n",value.token_type);
                break;
        }
    }
    varopts->value = var_value.file;
}
