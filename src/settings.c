#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "settings.h"
#ifndef SETTINGS_IMPL
#include "parser/parser.h"
#include "compiler.h"

IdentType SETTINGS_get_identtype(Expression *P_expr) {
    for (int token = 0; token < (P_expr->size/sizeof(Token)); ++token) {
        if (P_expr->tokens[token].token_type == TokenType_Ident) {
            IdentType res = get_ident_type(P_expr->tokens[token].value);
            if (res != IdentType_var_name) {
                return res;
            }
        }
    }
    assert(0 && "ERROR: could not find a keyword while parsing setting");
}

#endif
