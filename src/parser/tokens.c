#include "../contents.c"
#include "parser.h"
#ifndef TOKENSIMPL
Contents token_parse_expression_until(Expression *P_expr, int start, TokenType end_token) {
    Contents string = CONTENTS_new();
    for (int text = start; text < (P_expr->size / sizeof(Token)); ++text) {
        Token *token = &P_expr->tokens[text];
        if (token->token_type == end_token) {
            break;
        }
        if (token->value) {
            CONTENTS_append_str(&string, token->value);
        } else {
            CONTENTS_append(&string, token->character);
        }
    }
    return string;
}
Contents token_parse_expression_to_end(Expression *P_expr, int start) {
    return token_parse_expression_until(P_expr, start, TokenType_None);
}
Contents token_string_parse(Expression *P_expr, int start) {
    Contents con = token_parse_expression_until(P_expr, start, TokenType_DoubleQuote);
    return con;
}

int EXPRESSION_token_exist(Expression *P_expr,int start,TokenType token) {
    for (;start < ((P_expr->size)/sizeof(Token));++start) {
        if (P_expr->tokens[start].token_type == token) {
            return start;
        } else if (P_expr->tokens[start].token_type == TokenType_Space) {
            continue;
        // this means it is not white space and a completly different token
        } else {
            return -1;
        }
    }
    return -1;
}
void CONTENTS_append_token(Contents *P_con, Token token) {
    if (token.token_type == TokenType_Ident) {
        CONTENTS_append_str(P_con, token.value);
    } else {
        CONTENTS_append(P_con, token.character);
    }
}
#endif
