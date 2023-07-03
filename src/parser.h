#include <stdlib.h>
#include <sys/types.h>
#ifndef TOKENS
#define TOKENS

typedef enum TokenType {
    TokenType_Ident,
    TokenType_LeftParenthesis,
    TokenType_RightParenthesis,
    TokenType_LeftBracket,
    TokenType_RightBracket,
    TokenType_DoubleQuote,
    TokenType_SingleQuote,
    TokenType_DollarSign,
    TokenType_AtSign,
    TokenType_EqualSign,
    TokenType_Space,
} TokenType;
typedef enum IdentType {
    IdentType_println,
    IdentType_printfln,
    IdentType_print,
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
    char character;
} Token;

typedef struct Expression {
    Token* tokens;
    uint size;
    uint max;
} Expression;

Expression EXPRESSION_new();
void EXPRESSIONappend(Expression *P_expr, Token token);
int EXPRESSION_token_exist(Expression *P_expr,int start,TokenType token);

typedef struct Expressions {
    Expression* exprs;
    uint size;
    uint max;
} Expressions;

Expressions EXPRESSIONS_new();
void EXPRESSIONS_append(Expressions* P_exprs,Expression expr);
void EXPRESSIONS_drop(Expressions exprs);
#endif
