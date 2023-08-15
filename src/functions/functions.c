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
#include "../settings.h"
#include "../lib_tools.h"

FuncOpt FUNCTION_new() {
    FuncOpt opts;
    opts.args = NULL;
    opts.name = NULL;
    opts.return_type = NULL;
    opts.method = NULL;
    opts.fn_type = FUNCTYPE_None;
    opts.inline_fn = 0;
    opts.is_public = 0;
    return opts;
}

int settings_parse(Expression *P_expr, FuncOpt *opts) {
    int index = 0;
    int setting;
    do {
        setting = TOKEN_expression_index(P_expr, index, TokenType_AtSign);
        if (setting != -1) {
            Token setting_type = P_expr->tokens[setting + 1];
            assert(setting_type.token_type == TokenType_Ident && "ERROR: expected token ident after token '@'");

            SETTING_VAR_PARSE(setting_type, P_expr,"return",index,setting, opts,return_type, 0);
            SETTING_VAR_PARSE(setting_type, P_expr, "method", index, setting, opts, method, 0);

            SETTING_VALUE_PARSE(setting_type, "main", opts, index, fn_type, FUNCTYPE_main);
            SETTING_VALUE_PARSE(setting_type, "inline", opts, index, inline_fn, 1);
            SETTING_VALUE_PARSE(setting_type, "public", opts, index, is_public, 1);
        }
    } while (setting != -1);
    // returns two if index is -1 (no settings to parse)
    return !index ? 1 : index + 2;
}

FuncOpt FUNCTION_parse(Expression *P_expr) {
    // contains all of the nessesary data to create a new function
    FuncOpt opts = FUNCTION_new();

    // parses all of the settings args aka @name tokens
    int func_index = settings_parse(P_expr, &opts);
    char *fn_name = P_expr->tokens[func_index].value;
    printf("fn name type %i\n", P_expr->tokens[func_index].token_type);
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
        Contents new_name = CONTENTS_new();
        CONTENTS_append_formatted(&new_name, "__METHOD_%s%s",opts.method,opts.name);
        opts.name = new_name.file;
        if (strstr(args.file, "self") != NULL) {
            // replace the first occurace of self with the the type the method is being implemented for
            Contents new_args = CONTENTS_replace(&args, "self", opts.method);
            CONTENTS_drop(args);
            opts.args = new_args.file;
        }
    }
    return opts;
}

void FUNCTION_write_to_file(Compiler *P_comp, FuncOpt opt, char* module_name) {
    if (opt.is_public) {
        CONTENTS_append_formatted(&P_comp->contents, "#ifdef ___MODULE_%sBRANCH__%s\n",module_name, opt.name);
        IS_PUBLIC = 1;
    }
    if (opt.fn_type == FUNCTYPE_main) {
        INITED_MAIN = 1;
        // if it contians the main decorator ignore all of the arguments and function name
        char* heap_alloc = INITED_HEAP_ARRAY ? " __HeapArray *___heap": "";
        CONTENTS_append_formatted (&P_comp->contents, "int __MAIN(%s) {\n", heap_alloc);
        free(opt.args);
        return;
    }
    if (opt.inline_fn) { CONTENTS_append_str(&P_comp->contents, "static inline "); }
    // If there is no return type given set the return type to null
    if (opt.return_type == NULL) {
        CONTENTS_append_formatted (&P_comp->contents, "void __MODULE%s%s(%s) {\n",module_name, opt.name, opt.args);
    } else {
        CONTENTS_append_formatted (&P_comp->contents, "%s __MODULE%s%s(%s) {\n", opt.return_type,module_name, opt.name, opt.args);
    }
    if (opt.method) { free(opt.name); }
    free(opt.args);
}
#endif
