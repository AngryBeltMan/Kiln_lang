#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "tokens.h"
#include "../contents.c"
#ifndef TOKENMATCH
#define EXP_APPEND(expr,token) /*printf("appended from fn: %s on line: %i\n",__FUNCTION__,__LINE__); */ EXPRESSIONappend(expr,token);

void options_check(int open_char_count,char* value);

#define TOKENMATCHEXIT(match_char,type) \
    case match_char:\
        if (identifier == 1) {\
            token.value = value.file;\
            token.token_type = TokenType_Ident;\
            EXP_APPEND(&expr, token); \
            value = CONTENTS_new();\
        }\
        identifier = 0;\
        containers_count = 0;\
        open_quote = 0;\
        token.value = NULL;\
        token.character = match_char;\
        token.token_type = type;\
        EXP_APPEND(&expr,token);\
        EXPRESSIONS_append(&exprs, expr);\
        expr = EXPRESSION_new();\
        break;

#define TOKENSTRMATCH(name,match_str,tokentype)\
    if (!strcmp(name,match_str)) {\
        token.value = name;\
        token.token_type = tokentype;\
        EXP_APPEND(&expr,token);\
        value = CONTENTS_new(); \
        identifier = 0; \
        double_char = 1; \
    }

#define TOKENAPPEND(match_char,type)\
    token.value = NULL;\
    token.character = match_char;\
    token.token_type = type;\
    EXP_APPEND(&expr,token);

#define TOKENMATCH(match_char,type)\
    case match_char:\
        CONTENTSCHECK();\
        TOKENAPPEND(match_char, type);\
        break;

#define CONTENTSAPPEND() \
    int double_char = 0;\
    /* Token str match with set double char to one if they match with the second argument*/ \
    TOKENSTRMATCH(value.file,"->",TokenType_RightArrow);\
    TOKENSTRMATCH(value.file,"<-",TokenType_LeftArrow);\
    TOKENSTRMATCH(value.file,">>",TokenType_method_call);\
    TOKENSTRMATCH(value.file,"::",TokenType_method_call_no_self);\
    /*Only append the ident if it doesn't match with the any of the TOKENSTRMATCH*/ \
    if (double_char == 0) {\
        options_check(containers_count, value.file);\
        token.value = value.file;\
        token.character = ' ';\
        token.token_type = TokenType_Ident;\
        EXPRESSIONappend(&expr,token);\
        value = CONTENTS_new();\
        identifier = 0;\
    }

#define CONTENTSCHECK()\
    if (identifier) {\
        CONTENTSAPPEND()\
    }
#endif
#ifndef PARSERIMPL
#define PARSERIMPL
const int DROPPINGDEBUGINFO = 1;

void options_check(int open_char_count,char* value) {
    if (open_char_count == 0) {
        if (!strcmp(value, "heap")) {
            INITED_HEAP_ARRAY = 1;
        }
    }
}
Expression EXPRESSION_new() {
    Expression tokens;
    tokens.size = 0;
    tokens.tokens = malloc(sizeof(Token));
    if (tokens.tokens == NULL) { printf("ERROR: failed to malloc expression %s\n",__FUNCTION__); }
    tokens.max = sizeof(Token);
    return tokens;
}

void EXPRESSIONappend(Expression* P_expr,Token token) {
    if (P_expr->size + sizeof(Token) > P_expr->max) {
        P_expr->max *= 2;
        P_expr->tokens = realloc(P_expr->tokens,P_expr->max);
        if (P_expr->tokens == NULL) { printf("ERROR: failed to realloc expression %s\n",__FUNCTION__); }
    }
    P_expr->tokens[(P_expr->size)/sizeof(Token)] = token;
    P_expr->size += sizeof(Token);
}

Expressions EXPRESSIONS_new() {
    Expressions exprs;
    exprs.size = 0;
    exprs.exprs = malloc(sizeof(Expression));
    if (exprs.exprs == NULL) { printf("ERROR: failed to malloc expressions %s\n",__FUNCTION__); }
    exprs.max = sizeof(Expression);
    return exprs;
}

void EXPRESSIONS_append(Expressions* P_exprs,Expression expr) {
    if (P_exprs == NULL) {return;}
    if (P_exprs->size + sizeof(Expression) > P_exprs->max) {
        P_exprs->max *= 2;
        P_exprs->exprs = realloc(P_exprs->exprs,P_exprs->max);
        if (P_exprs->exprs == NULL) { printf("ERROR: failed to realloc expressions %s\n",__FUNCTION__); }
    }
    P_exprs->exprs[(P_exprs->size)/sizeof(Expression)] = expr;
    P_exprs->size += sizeof(Expression);
}
// itearates and frees all of the tokens
void EXPRESSIONS_drop(Expressions exprs) {
    for (int expr = 0;expr < (exprs.size/sizeof(Expression)); ++expr) {
        for (int token = 0; token < (exprs.exprs[expr].size/sizeof(Token));++token) {
            if (exprs.exprs[expr].tokens[token].value != NULL) {
                if (DROPPINGDEBUGINFO) { printf("dropping %s \n",exprs.exprs[expr].tokens[token].value); }
                free(exprs.exprs[expr].tokens[token].value);
                exprs.exprs[expr].tokens[token].value = NULL;
            } else {
                if (DROPPINGDEBUGINFO) { printf("dropping '%c'\n",exprs.exprs[expr].tokens[token].character); }
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

Expressions EXPRESSIONS_from_file(FILE *P_file) {
    int ch;
    Contents value = CONTENTS_new();
    Expression expr = EXPRESSION_new();
    Expressions exprs = EXPRESSIONS_new();
    Token token;
    token.value = NULL;

    int identifier = 0;
    // will decide whether or not to parse or ignore the spaces
    int containers_count = 0;
    int open_quote = 0;

    while ((ch = fgetc(P_file)) != EOF /* End of file char*/) {
        switch ((char)ch) {
            case '\n':
                CONTENTSCHECK();
                break;
            case ';':
                if (identifier == 1) {
                    token.value = value.file;
                    token.token_type = TokenType_Ident;
                    EXP_APPEND(&expr, token);
                    value = CONTENTS_new();
                }
                identifier = 0;
                EXPRESSIONS_append(&exprs, expr);
                expr = EXPRESSION_new();
                break;
            case '"':
                CONTENTSCHECK();
                if (open_quote) { --containers_count; } else {++containers_count;}
                open_quote = !open_quote;
                TOKENAPPEND('"',TokenType_DoubleQuote);
                break;
            case '(':
                CONTENTSCHECK();
                ++containers_count;
                TOKENAPPEND('(',TokenType_LeftParenthesis);
                break;
            case ')':
                CONTENTSCHECK();
                --containers_count;
                TOKENAPPEND(')',TokenType_RightParenthesis);
                break;
            case ' ':
                CONTENTSCHECK();
                if (containers_count == 0) { break; }
                TOKENAPPEND(' ',TokenType_Space);
                break;
            TOKENMATCHEXIT('}', TokenType_RightBracket)
            TOKENMATCHEXIT('{', TokenType_LeftBracket)
            TOKENMATCH('$', TokenType_DollarSign)
            TOKENMATCH('@', TokenType_AtSign)
            TOKENMATCH('=', TokenType_EqualSign)
            TOKENMATCH(',', TokenType_Comma)
            default:
                identifier = 1;
                CONTENTS_append(&value,ch);
                break;
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
#endif
