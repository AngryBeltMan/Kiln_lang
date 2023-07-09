#include "calls.h"
#include <stdio.h>
#ifndef CALLDEF
#include "../compiler.h"
#include "../contents.c"

CallsType CALLSTYPE_parse(Expression *P_expr, int start_index) {
    CallsType call_type;
    int next_tok_type = P_expr->tokens[start_index + 1].token_type;
    switch (next_tok_type) {
        // if there is a paraenthesis it will be assumed to be a function call
        // EXAMPLE: main();
        //              ^
        //              next token is a left parenthesis
        case TokenType_LeftParenthesis: {
            char* func_name = P_expr->tokens[start_index].value;
            printf("function name %s\n",func_name);
            // gets the function arguments
            Contents args = token_parse_expression_until(P_expr, start_index + 2,
                    TokenType_RightParenthesis);
            call_type.Type.FnCall.args =args.file;
            printf("function arg\n");
            call_type.Type.FnCall.fn_name = func_name;
            call_type.CallTypeArm = CALLTYPEARM_fn_call;
            break;
        }
        case TokenType_EqualSign: {
            call_type.CallTypeArm = CALLTYPEARM_var_assign;
            call_type.Type.VarAssign.var_name = P_expr->tokens[start_index].value;
            call_type.Type.VarAssign.value = malloc(8);
            *call_type.Type.VarAssign.value = CALLSTYPE_parse(P_expr, start_index + 2);
            break;
        }
        default: {
            call_type.Type.Value.value = P_expr->tokens[start_index + 1].value;
            call_type.CallTypeArm = CALLTYPEARM_value;
            break;
        }
    }
    return call_type;
}

void CALLSTYPE_write_to_file(Compiler *P_comp, CallsType call_type) {
    switch (call_type.CallTypeArm) {
        case CALLTYPEARM_var_assign: {
            CONTENTS_append_formated(&P_comp->contents, "%s = ", call_type.Type.VarAssign.var_name);
            CALLSTYPE_write_to_file(P_comp, *call_type.Type.VarAssign.value);
            break;
        }
        case CALLTYPEARM_value: {
            CONTENTS_append_formated(&P_comp->contents, "%s;\n", call_type.Type.Value.value);
            break;
        }
        case CALLTYPEARM_fn_call: {
            CONTENTS_append_formated(&P_comp->contents, "%s(%s);\n",call_type.Type.FnCall.fn_name, call_type.Type.FnCall.args );
            break;
        }
        case CALLTYPEARM_arth: {
            break;
        }
    }
    CONTENTS_append_formated(&P_comp->contents, "");
}
void CALLSTYPE_free(CallsType call_type) {
    switch (call_type.CallTypeArm) {
        case CALLTYPEARM_var_assign:
            CALLSTYPE_free(*call_type.Type.VarAssign.value);
            free(call_type.Type.VarAssign.value);
            break;
        case CALLTYPEARM_fn_call: {
            free(call_type.Type.FnCall.args);
            break;
        }
        // unimplemented
        case CALLTYPEARM_arth: {
            break;
        }
        case CALLTYPEARM_value: {
            break;
        }

    }

}
#endif
