#include "functions.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef FUNCTIONSIMPL
#define FUNCTIONSIMPL
#include "../compiler.h"
#include "../parser.h"
#include "../variables/variables.h"

FuncOpt FUNCTION_new() {
    FuncOpt opts;
    opts.args = NULL;
    opts.name = NULL;
    opts.fn_type = FUNCTYPE_None;
    opts.return_type = NULL;
    return opts;
}

int settings_parse(Expression *P_expr, FuncOpt *opts) {
    int index = 0;
    int setting;
    do {
        setting = EXPRESSION_token_exist(P_expr, index, TokenType_AtSign);
        if (setting != -1) {
            printf("setting\n");
            Token setting_type = P_expr->tokens[setting + 1];
            if (setting_type.token_type != TokenType_Ident) {
                printf("ERROR: expected token ident after token '@' %s\n",
                        __FUNCTION__);
            }
            if (!strcmp(setting_type.value, "return")) {
                index = setting;
                char *return_val = parse_setting_var(P_expr, &index);
                printf("return val %s\n", return_val);
                assert(return_val != NULL &&
                        "ERROR: return setting var requires a return type value\n");
                printf("return val %s\n", return_val);
                opts->return_type = return_val;
                index += 2;
            }
            if (!strcmp(setting_type.value, "main")) {
                printf("main\n");
                opts->fn_type = FUNCTYPE_main;
                index += 3;
            }
        }
    } while (setting != -1);
    printf("index %i\n", index);
    // returns two if index is -1 (no settings to parse)
    return index != 0 ? index : 1;
}

FuncOpt FUNCTION_parse(Expression *P_expr) {
    // contains all of the nessesary data to create a new function
    FuncOpt opts = FUNCTION_new();

    // parses all of the settings args aka @name tokens
    int func_index = settings_parse(P_expr, &opts);
    printf("fn index %i\n", func_index);
    char *fn_name = P_expr->tokens[func_index].value;
    assert(fn_name != NULL && "ERROR: could not parse function name.");
    // gets the index for the left parenthesis token
    ++func_index;
    int res = EXPRESSION_token_exist(P_expr, func_index, TokenType_LeftParenthesis);
    if (res == -1) {
        printf("ERROR: could not find left parenthesis while parsing function %s - " "%s\n", fn_name, __FUNCTION__);
    } else {
        func_index = res;
    }
    // continue to parse all of the tokens after the left parenthesis up until a
    // right parenthesis is encountered
    ++func_index;
    Contents args = token_parse_expression_until(P_expr, func_index,
            TokenType_RightParenthesis);

    // add all of the function data in the opts struct
    opts.args = args.file;
    printf("fn args %s\n", args.file);
    opts.name = fn_name;
    return opts;
}

void FUNCTION_write_to_file(Compiler *P_comp, FuncOpt opt) {
    if (opt.fn_type == FUNCTYPE_main) {
        char* heap_alloc = INITED_HEAP_ARRAY ? " __HeapArray *___heap": "";
        CONTENTS_append_formated(&P_comp->contents, "int __MAIN(%s) {\n", heap_alloc);
        return;
    }
    if (opt.return_type == NULL) {
        CONTENTS_append_formated(&P_comp->contents, "void %s(%s) {\n", opt.name, opt.args);
    } else {
        CONTENTS_append_formated(&P_comp->contents, "%s %s(%s) {\n", opt.return_type,
                opt.name, opt.args);
    }
    free(opt.args);
}
#endif
