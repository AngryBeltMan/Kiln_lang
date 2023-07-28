#include "if_statement.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef IFSTATEMENT
#ifndef IFSTATEMENTIMPL
#include "../compiler/compiler.h"
#include "../parser/parser.h"
#include "../contents.c"
#define IFSTATEMENTIMPL

void IfStatement_get_contents(Contents* P_con, Expression* P_expr, int *P_index);

IfStatementOpt IFSTATEMENT_parse(Expression* P_expr) {
    IfStatementOpt opts;
    opts.value = NULL;
    Contents cmp_statement = CONTENTS_new();
    int start = EXPRESSION_token_exist(P_expr, 1, TokenType_LeftParenthesis) + 1;
    if ( start == -1) {
        printf("ERROR: failed to parse if statement %s\n",__FUNCTION__);
        abort();
    }
    IfStatement_get_contents(&cmp_statement,P_expr,&start);
    opts.value = cmp_statement.file;
    return opts;
}

void IFSTATEMENT_write_to_file(Compiler* P_comp,IfStatementOpt statement) {
    CONTENTS_append_formatted (&P_comp->contents,"if (%s {\n",statement.value);
    free(statement.value);
}

void IfStatement_get_contents(Contents* P_con, Expression* P_expr, int *P_index) {
    int parenthesis_count = 1;
    for (; *P_index < (P_expr->size/sizeof(Token)); ++*P_index) {
        Token token = P_expr->tokens[*P_index];
        switch (token.token_type) {
            case TokenType_RightParenthesis:
                CONTENTSappend(P_con,')');
                --parenthesis_count;
                if (parenthesis_count == 0) { return; }
                continue;
            case TokenType_LeftParenthesis:
                ++parenthesis_count;
                CONTENTSappend(P_con,'(');
                continue;
            case TokenType_Ident:
                if (token.value) {
                    CONTENTS_append_str(P_con,token.value);
                } else  {
                    // should never happen
                    printf("ERROR: if statement value is NULL %s\n", __FUNCTION__);
                }
                continue;
            default:
                CONTENTSappend(P_con,token.character);
                continue;

        }
    }
}
#endif
#endif
