#include <stdlib.h>
#include <sys/types.h>
#ifndef PARSER
#define PARSER

// setting vars
static int INITED_HEAP_ARRAY = 0;

typedef enum TokenType {
    TokenType_Ident,
    TokenType_LeftParenthesis,
    TokenType_RightParenthesis,
    TokenType_LeftBracket,
    TokenType_RightBracket,
    TokenType_DoubleQuote,
    TokenType_SingleQuote,
    TokenType_DollarSign,
    TokenType_Comma,
    TokenType_AtSign,
    TokenType_EqualSign,
    TokenType_Space,
    // Nonexistant Token
    TokenType_None,
    // double char tokens
    TokenType_RightArrow,
    TokenType_LeftArrow,
    TokenType_Addition,
    TokenType_Subtraction,
    TokenType_Multiplication,
    TokenType_Division,
} TokenType;
typedef enum IdentType {
    IdentType_println,
    IdentType_printfln,
    IdentType_print,
    IdentType_printf,
    IdentType_var_init,
    IdentType_var_name,
    IdentType_if_statement,
    IdentType_break_bracket,
    IdentType_break_scope,
    IdentType_return_fn,
    IdentType_struct_init,
    IdentType_function,
    IdentType_for_loop,
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
