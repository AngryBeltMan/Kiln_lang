#include "calls.h"
#include <stdio.h>
#ifndef CALLDEF
#include "../compiler/compiler.h"
#include "../contents.c"

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
            call_type.CallTypeArm = CALLTYPEARM_fn_call;
            break;
        }
        case TokenType_EqualSign: {
            call_type.CallTypeArm = CALLTYPEARM_var_assign;
            call_type.Type.VarAssign.var_name = P_expr->tokens[start_index].value;
            call_type.Type.VarAssign.value = malloc(24);
            if ( P_expr->size / 16 > (start_index + 3)) {
                printf("funcy calfdlsfjkldsajkfldsjklfjklasfjdkljfkdlsajfkldjfkldsjfkldsfjkdsjfkldsajl\n");
                *call_type.Type.VarAssign.value = CALLSTYPE_parse(P_expr, var_map, start_index + 2);
            } else {
                *call_type.Type.VarAssign.value = CALLSTYPE_parse(P_expr, var_map, start_index + 1);
            }
            break;
        }
        case TokenType_method_call: {
            printf("method call\n");
            Token var_name = P_expr->tokens[start_index];
            assert(var_name.token_type == TokenType_Ident && "Error: expected name after");
            VarData* var_type = (VarData*)hashmap_get(var_map, &(VarData){.name = var_name.value});
            Contents args = token_parse_expression_until(P_expr, start_index + 4, TokenType_RightParenthesis);
            Contents modified_args = CONTENTS_new();
            if (args.size == 0) {
                CONTENTS_append_formatted(&modified_args, "%s",var_name.value);
            } else {
                CONTENTS_append_formatted(&modified_args, "%s,%s",var_name.value, args.file);
            }
            CONTENTS_drop(args);
            Contents function_name = CONTENTS_new();
            CONTENTS_append_formatted(&function_name, "__METHOD_%s%s",var_type->type, P_expr->tokens[start_index + 2] );
            call_type.Type.FnCall.args = modified_args.file;
            call_type.Type.FnCall.fn_name = function_name.file;
            call_type.CallTypeArm = CALLTYPEARM_fn_call;
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
