#include "functions.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef FUNCTIONSIMPL
#define FUNCTIONSIMPL
#include "../compiler/compiler.h"
#include "../parser/parser.h"
#include "../parser/tokens.h"
#include "../variables/variables.h"

FuncOpt FUNCTION_new() {
    FuncOpt opts;
    opts.args = NULL;
    opts.name = NULL;
    opts.return_type = NULL;
    opts.method = NULL;
    opts.fn_type = FUNCTYPE_None;
    opts.inline_fn = 0;
    return opts;
}

int settings_parse(Expression *P_expr, FuncOpt *opts) {
    int index = 0;
    int setting;
    do {
        setting = TOKEN_expression_index(P_expr, index, TokenType_AtSign);
        /* setting = EXPRESSION_token_exist(P_expr, index, TokenType_AtSign); */
        printf("setting %i\n",setting);
        if (setting != -1) {
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
            }
            if (!strcmp(setting_type.value, "method")) {
                index = setting;
                char *method_for = parse_setting_var(P_expr, &index);
                assert(method_for != NULL &&
                        "ERROR: return setting var requires a return type value\n");
                opts->method = method_for;
            }
            if (!strcmp(setting_type.value, "main")) {
                opts->fn_type = FUNCTYPE_main;
                index += 1;
            }
            if (!strcmp(setting_type.value, "inline")) {
                opts->inline_fn = 1;
                index += 1;
            }
        }
    } while (setting != -1);
    printf("index %i\n", index);
    // returns two if index is -1 (no settings to parse)
    return index == -1 ? 1 : index + 2;
}

FuncOpt FUNCTION_parse(Expression *P_expr) {
    // contains all of the nessesary data to create a new function
    FuncOpt opts = FUNCTION_new();

    // parses all of the settings args aka @name tokens
    int func_index = settings_parse(P_expr, &opts);
    printf("fn index %i\n", func_index);
    char *fn_name = P_expr->tokens[func_index].value;
    printf("%c\n", P_expr->tokens[func_index + 1].character);
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
    Contents args = token_parse_expression_until(P_expr, func_index, TokenType_RightParenthesis);
    // if there is no arguments add void
    // this is done so the args value is atleast initialized
    if (args.size == 0) { CONTENTS_append_str(&args,"void"); }

    // add all of the function data in the opts struct
    opts.args = args.file;
    opts.name = fn_name;

    if (opts.method != NULL) {
        if (strstr(args.file, "self") != NULL) {
            // replace the first occurace of self with the the type the method is being implemented for
            Contents new_args = CONTENTS_replace(&args, "self", opts.method);
            Contents new_name = CONTENTS_new();
            CONTENTS_append_formatted(&new_name, "__METHOD_%s%s",opts.method,opts.name);
            opts.name = new_name.file;
            CONTENTS_drop(args);
            opts.args = new_args.file;
        }
    }
    return opts;
}

void FUNCTION_write_to_file(Compiler *P_comp, FuncOpt opt) {
    if (opt.fn_type == FUNCTYPE_main) {
        // if it contians the main decorator ignore all of the arguments and function name
        char* heap_alloc = INITED_HEAP_ARRAY ? " __HeapArray *___heap": "";
        CONTENTS_append_formatted (&P_comp->contents, "int __MAIN(%s) {\n", heap_alloc);
        free(opt.args);
        return;
    }
    if (opt.inline_fn) { CONTENTS_append_str(&P_comp->contents, "static inline "); }
    // If there is no return type given set the return type to null
    if (opt.return_type == NULL) {
        CONTENTS_append_formatted (&P_comp->contents, "void %s(%s) {\n", opt.name, opt.args);
    } else {
        CONTENTS_append_formatted (&P_comp->contents, "%s %s(%s) {\n", opt.return_type,
                opt.name, opt.args);
    }
    if (opt.method) { free(opt.name); }
    free(opt.args);
}
#endif
