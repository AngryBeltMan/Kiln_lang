#include "calls.h"
#include <stdio.h>
#ifndef CALLDEF
#include "../compiler/compiler.h"
#include "../contents.c"

void method_write_to_file(Expression *P_expr,CallsType *P_calls, Hashmap *var_map, int reference, int start_index, int include_self) {
    Token var_name = P_expr->tokens[start_index];
    assert(var_name.token_type == TokenType_Ident && "Error: expected name after");
    // Vae map will be used to look up the type of the variable in order to know which method to call
    VarData* var_type = (VarData*)hashmap_get(var_map, &(VarData){.name = var_name.value});
    Contents args = token_parse_expression_until(P_expr, start_index + 4, TokenType_RightParenthesis);
    Contents modified_args = CONTENTS_new();
    if (reference) { CONTENTS_append(&modified_args, '&'); }

    if (args.size == 0) {
        CONTENTS_append_formatted(&modified_args, "%s",var_name.value);
    } else {
        CONTENTS_append_formatted(&modified_args, "%s%c%s",include_self ? var_name.value: "", include_self? ',': ' ', args.file);
    }

    CONTENTS_drop(args);
    Contents function_name = CONTENTS_new();
    CONTENTS_append_formatted(&function_name, "__METHOD_%s%s",var_type->type, P_expr->tokens[start_index + 2] );
    P_calls->Type.FnCall.args = modified_args.file;
    P_calls->Type.FnCall.fn_name = function_name.file;
    P_calls->CallTypeArm = CALLTYPEARM_fn_call;
    P_calls->is_method = 1;
}

CallsType CALLSTYPE_parse(Expression *P_expr, Hashmap *var_map, int start_index) {
    CallsType call_type;
    int next_tok_type = P_expr->tokens[start_index + 1].token_type;
    switch (next_tok_type) {
        // if there is a paraenthesis it will be assumed to be a function call
        // EXAMPLE: main();
        //              ^
        //              next token is a left parenthesis
        case TokenType_LeftParenthesis: {
            char* func_name = P_expr->tokens[start_index].value;
            // gets the function arguments
            Contents args = token_parse_expression_until(P_expr, start_index + 2, TokenType_RightParenthesis);
            if (args.size == 0) { CONTENTS_append(&args,' '); }
            call_type.Type.FnCall.args = args.file;
            call_type.Type.FnCall.fn_name = func_name;
            call_type.is_method = 0;
            call_type.CallTypeArm = CALLTYPEARM_fn_call;
            break;
        }
        case TokenType_EqualSign: {
            call_type.CallTypeArm = CALLTYPEARM_var_assign;
            call_type.Type.VarAssign.var_name = P_expr->tokens[start_index].value;
            call_type.Type.VarAssign.value = malloc(24);
            // 16 is the size of the Token
            if ( P_expr->size / 16 > (start_index + 3)) {
                *call_type.Type.VarAssign.value = CALLSTYPE_parse(P_expr, var_map, start_index + 2);
            } else {
                *call_type.Type.VarAssign.value = CALLSTYPE_parse(P_expr, var_map, start_index + 1);
            }
            break;
        }
        case TokenType_method_call: {
            method_write_to_file(P_expr, &call_type, var_map, 1, start_index, 1);
            break;
        }
        case TokenType_RightArrow: {
            method_write_to_file(P_expr, &call_type, var_map, 0, start_index, 1);
            break;
        }
        case TokenType_method_call_no_self: {
            method_write_to_file(P_expr, &call_type, var_map, 0, start_index, 0);
            break;
        }
        case TokenType_Ident: {
            printf("ident call\n");
            call_type.Type.Value.value = P_expr->tokens[start_index + 1].value;
            call_type.CallTypeArm = CALLTYPEARM_value;
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
            CONTENTS_append_formatted (&P_comp->contents, "%s = ", call_type.Type.VarAssign.var_name);
            CALLSTYPE_write_to_file(P_comp, *call_type.Type.VarAssign.value);
            break;
        }
        case CALLTYPEARM_value: {
            CONTENTS_append_formatted (&P_comp->contents, "%s;\n", call_type.Type.Value.value);
            break;
        }
        case CALLTYPEARM_fn_call: {
            CONTENTS_append_formatted (&P_comp->contents, "%s(%s);\n",call_type.Type.FnCall.fn_name, call_type.Type.FnCall.args );
            break;
        }
        case CALLTYPEARM_arth: {
            break;
        }
    }
    CONTENTS_append_formatted (&P_comp->contents, "");
}
void CALLSTYPE_free(CallsType call_type) {
    switch (call_type.CallTypeArm) {
        case CALLTYPEARM_var_assign:
            CALLSTYPE_free(*call_type.Type.VarAssign.value);
            free(call_type.Type.VarAssign.value);
            break;
        case CALLTYPEARM_fn_call: {
            printf("freeing\n");
            free(call_type.Type.FnCall.args);
            if (call_type.is_method) { free(call_type.Type.FnCall.fn_name); }
            printf("freeing success\n");
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
