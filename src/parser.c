#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "contents.c"
#ifndef TOKENMATCH
#define TOKENMATCH(char,type)\
    case char:\
        terminate = 1;\
        token.value = NULL;\
        printf("appending type %i\n",type); \
        token.token_type = type;\
        printf("type is type %i\n",token.token_type); \
        EXPRESSION_append(&expr,token);\
        continue;
#endif
Expression EXPRESSION_new() {
    Expression tokens;
    tokens.size = 0;
    tokens.tokens = malloc(sizeof(Token));
    tokens.max = sizeof(Token);
    return tokens;
}
void EXPRESSION_append(Expression* P_expr,Token token) {
    printf("type is now %i\n",token.token_type);
    if (P_expr->size + sizeof(Token) > P_expr->max) {
        P_expr->max *= 2;
        P_expr->tokens = realloc(P_expr->tokens,P_expr->max);
    }
    P_expr->tokens[(P_expr->size)/sizeof(Token)] = token;
    P_expr->size += sizeof(Token);
}
void EXPRESSION_insert_behind(Expression* P_expr,Token token) {
    Token current_token = P_expr->tokens[(P_expr->size)/sizeof(Token)];
    P_expr->tokens[((P_expr->size)/sizeof(Token)) - 1] = token;
    EXPRESSION_append(P_expr, current_token);
}
Expressions EXPRESSIONS_new() {
    Expressions exprs;
    exprs.size = 0;
    exprs.exprs = malloc(sizeof(Expression));
    exprs.max = sizeof(Expression);
    return exprs;
}
void EXPRESSIONS_append(Expressions* P_exprs,Expression expr) {
    if (P_exprs == NULL) {return;}
    if (P_exprs->size + sizeof(Expression) > P_exprs->max) {
        P_exprs->max *= 2;
        P_exprs->exprs = realloc(P_exprs->exprs,P_exprs->max);
    }
    P_exprs->exprs[(P_exprs->size)/sizeof(Expression)] = expr;
    P_exprs->size += sizeof(Expression);
}
// itearates and frees all of the tokens
void EXPRESSIONS_drop(Expressions exprs) {
    for (int expr = 0;expr < (exprs.size/sizeof(Expression)); ++expr) {
        for (int token = 0; token < (exprs.exprs[expr].size/sizeof(Token));++token) {
            if (exprs.exprs[expr].tokens[token].value != NULL) {
                free(exprs.exprs[expr].tokens[token].value);
                exprs.exprs[expr].tokens[token].value = NULL;
            }
        }
        if (exprs.exprs[expr].tokens) {
            free(exprs.exprs[expr].tokens);
            exprs.exprs[expr].tokens = NULL;
        }
    }
    if (exprs.exprs) {
        free(exprs.exprs);
        exprs.exprs = NULL;
    }
}
void Expression_add_context(Token* P_token,Expression*P_expr,Contents* P_value) {
    P_token->value = P_value->file;
    P_token->token_type = TokenType_Ident;
    EXPRESSION_insert_behind(P_expr, *P_token);
}
Expressions EXPRESSIONS_from_file(FILE *P_file) {
    int ch;
    Contents value = CONTENTS_new();
    Expression expr = EXPRESSION_new();
    Expressions exprs = EXPRESSIONS_new();
    Token token;
    token.value = NULL;

    int terminate = 0;
    int identifier = 0;

    while ((ch = fgetc(P_file)) != EOF /* End of file char*/) {
        if (terminate && identifier) {
            Expression_add_context(&token, &expr, &value);
            value = CONTENTS_new();
            identifier = 0;
            terminate = 0;
        }
        switch ((char)ch) {
            case ';':
                if (identifier == 1) {
                    token.value = value.file;
                    token.token_type = TokenType_Ident;
                    EXPRESSION_append(&expr, token);
                    value = CONTENTS_new();
                }
                identifier = 0;
                terminate = 0;
                EXPRESSIONS_append(&exprs, expr);
                expr = EXPRESSION_new();
                continue;
            TOKENMATCH('(', TokenType_LeftParenthesis)
            TOKENMATCH(')', TokenType_RightParenthesis)
            TOKENMATCH('"', TokenType_DoubleQuote)
            TOKENMATCH('$', TokenType_DollarSign)
            TOKENMATCH('=', TokenType_EqualSign)
            TOKENMATCH(' ', TokenType_Space)
            case '\n':
                continue;
            default:
                identifier = 1;
                terminate = 0;
                CONTENTS_append(&value,ch);
                continue;
        }
    }
    for (int e = 0; e < expr.size/sizeof(Token); ++e) {
        if (expr.tokens[e].value) {
            free(expr.tokens[e].value);
            expr.tokens[e].value = NULL;
        }
    }
    if (value.file != NULL) { free(value.file); value.file = NULL; }
    if (expr.tokens != NULL) { free(expr.tokens); expr.tokens = NULL; }
    return exprs;
}
