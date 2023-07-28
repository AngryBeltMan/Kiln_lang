#include "structs.h"
#include <assert.h>
#include <stdio.h>
#include "../parser/tokens.h"
#ifndef STRUCTSIMPL
#define STRUCTSIMPL
// Creates a new empty struct instance
StructOpt STRUCT_new() {
    StructOpt struct_opt;
    struct_opt.struct_args = NULL;
    struct_opt.struct_name = NULL;
    return struct_opt;
}

void STRUCT_write_to_file(Compiler* P_comp, StructOpt opts) {
    CONTENTS_append_formatted (&P_comp->contents, "typedef struct %s { %s; } %s;\n",opts.struct_name,opts.struct_args,opts.struct_name);
    // Don't need to free opts.struct_name because it is a referense to a P_expr value an therefore will be dealloced in another time
    if (opts.struct_args) { free(opts.struct_args); }
}
// SYNTAX:
// struct <name> (<args>);

StructOpt STRUCT_parse_file(Expression *P_expr) {
    StructOpt opts = STRUCT_new();
    char* name = P_expr->tokens[1].value;

    assert(name != NULL && "ERROR: could not find name for struct");
    printf("struct name %s\n",name);

    Contents args = CONTENTS_new();

    for (int token_index = 3; token_index < (P_expr->size/sizeof(Token)); ++token_index) {
        Token arg_token = P_expr->tokens[token_index];
        if (arg_token.token_type == TokenType_RightParenthesis) { break; }
        if (arg_token.token_type != TokenType_Ident) {
            int res = (arg_token.character == ',');
            CONTENTS_append(&args,  res ? ';': arg_token.character );
        } else {
            CONTENTS_append_str(&args, arg_token.value);
        }
    }

    if (args.size == 0) { CONTENTS_append(&args, ' '); }
    opts.struct_args = args.file;
    opts.struct_name = name;
    return opts;
}
#endif
