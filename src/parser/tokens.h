#include "../contents.c"
#include "parser.h"

#ifndef PARSERTOKEN
#define PARSERTOKEN
// DOCS: parse the tokens in expression until it meets encounters a token with the same token type and the argument end_token.
// If it reaches the end and doesn't encounter a token with the same token type as end_token then it will return all the text from the start index to the end.
Contents token_parse_expression_until(Expression *P_expr, int start, TokenType end_token);

// DOCS: It will return all the text from the start index to the end.
Contents token_parse_expression_to_end(Expression *P_expr, int start);

// DOCS: parse the tokens in expression until it meets encounters a double quote.
Contents token_string_parse(Expression *P_expr, int start);

int EXPRESSION_token_exist(Expression *P_expr,int start,TokenType token);

void CONTENTS_append_token(Contents *P_con, Token token);

// DOCS: finds and returns the index where a certain token is located. Returns -1 if the token cannot be found in the expression.
static inline int TOKEN_expression_index(Expression *P_expr,int start,TokenType token) {
    for (;start < ((P_expr->size)/sizeof(Token));++start) {
        if (P_expr->tokens[start].token_type == token) {
            return start;
        }
    }
    return -1;
}

#endif
