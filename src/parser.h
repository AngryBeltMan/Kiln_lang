#include <stdlib.h>
#include <sys/types.h>
#ifndef TOKENS
#define TOKENS

typedef enum TokenType {
    TokenType_Ident = 0,
    TokenType_LeftParenthesis = 1,
    TokenType_RightParenthesis = 2,
    TokenType_LeftBracket = 3,
    TokenType_RightBracket = 4,
    TokenType_DoubleQuote = 5,
    TokenType_SingleQuote = 6,
    TokenType_DollarSign = 7,
    TokenType_EqualSign = 8,
    TokenType_Space = 9,
} TokenType;
typedef enum IdentType {
    IdentType_println,
    IdentType_printfln,
    IdentType_printf,
    IdentType_varname,
    IdentType_vartype,
    IdentType_var_init,
    IdentType_varg_var,
    IdentType_varg_ref,

} IdentType;

typedef struct Token {
    char* value;
    TokenType token_type;
} Token;

typedef struct Expression {
    Token* tokens;
    uint size;
    uint max;
} Expression;

Expression EXPRESSION_new();
void EXPRESSION_append(Expression *P_expr, Token token);

typedef struct Expressions {
    Expression* exprs;
    uint size;
    uint max;
} Expressions;

Expressions EXPRESSIONS_new();
void EXPRESSIONS_append(Expressions* P_exprs,Expression expr);
void EXPRESSIONS_drop(Expressions P_exprs);
#endif
