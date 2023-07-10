#include "compiler.h"
#include "../lib_tools.c"
#include "../for_loop/forloop.c"
#include "../for_loop/forloop.h"
#include "../settings.h"
#include "../settings.c"
#include "../if_statements/if_statement.c"
#include "../if_statements/if_statement.h"
#include "../variables/variables.c"
#include "../variables/variables.h"
#include "../functions/functions.h"
#include "../functions/functions.c"
#include "../calls/calls.h"
#include "../calls/calls.c"
#include "../parser/parser.h"
#include "../parser/parser.c"
#include "../parser/tokens.h"
#include "../parser/tokens.c"
#include "../structs/structs.h"
#include "../structs/structs.c"

#define TOKEN_CMP(ident, name, type)\
    if (!strcmp(ident, name)) {\
        return type;\
    }

IdentType get_ident_type(char *ident) {
    TOKEN_CMP(ident, "print", IdentType_print);
    TOKEN_CMP(ident, "println", IdentType_println);
    TOKEN_CMP(ident, "fprint", IdentType_printf);
    TOKEN_CMP(ident, "fprintln", IdentType_printf);
    TOKEN_CMP(ident, "let", IdentType_var_init);
    TOKEN_CMP(ident, "if", IdentType_if_statement);
    TOKEN_CMP(ident, "for", IdentType_for_loop);
    TOKEN_CMP(ident, "func", IdentType_function);
    TOKEN_CMP(ident, "ret", IdentType_return_fn);
    TOKEN_CMP(ident, "struct", IdentType_struct_init);
    TOKEN_CMP(&ident[0], "}", IdentType_break_bracket);
    printf("varname\n");
    return IdentType_var_name;
}

void write_print(Compiler *P_comp, Expression *P_expr, char *formatting) {
    COMPILER_add_module(P_comp, MODULE_stdio);
    Contents contents = token_string_parse(P_expr, 3);
    CONTENTS_append_formated(&P_comp->contents, "printf(\"%s%s", contents.file, formatting);
    free(contents.file);
    contents.file = NULL;
}

void parse_expression_from_keyword(IdentType ident_type, Compiler *P_comp, Expressions *P_exprs, int index) {

    /* switch (get_ident_type(token.value)) { */
    switch (ident_type) {
        // println("hello world");
        case IdentType_println:
            write_print(P_comp, &P_exprs->exprs[index], "\\n\");\n");
            break;
        case IdentType_print:
            write_print(P_comp, &P_exprs->exprs[index], "\");\n");
            break;
            // let num $int = 10;
        case IdentType_var_init:
            printf("initing\n");
            VarOpts varopts = VAROPTS_expression_parse(&P_exprs->exprs[index], P_comp);
            VAROPTS_create_var(varopts, P_comp);
            break;
        case IdentType_if_statement:
            printf("if statement\n");
            IfStatementOpt ifopts = IFSTATEMENT_parse(&P_exprs->exprs[index]);
            IFSTATEMENT_write_to_file(P_comp, ifopts);
            break;
        case IdentType_for_loop:
            printf("for loop\n");
            ForLoopOpt for_opts = FORLOOP_parse(&P_exprs->exprs[index]);
            FORLOOP_write_to_file(P_comp, for_opts);
            break;
        case IdentType_function:
            printf("function\n");
            FuncOpt func_opts = FUNCTION_parse(&P_exprs->exprs[index]);
            FUNCTION_write_to_file(P_comp, func_opts);
            break;
        case IdentType_var_name: {
            CallsType call = CALLSTYPE_parse(&P_exprs->exprs[index],0);
            CALLSTYPE_write_to_file(P_comp, call);
            CALLSTYPE_free(call);
            break;
        }
        case IdentType_return_fn: {
            char* return_val = token_parse_expression_to_end(&P_exprs->exprs[index],1).file;
            CONTENTS_append_formated(&P_comp->contents, "return %s;\n",return_val);
            free(return_val);
            break;
        }
        case IdentType_struct_init: {
            StructOpt opts = STRUCT_parse_file(&P_exprs->exprs[index]);
            STRUCT_write_to_file(P_comp, opts);
        }
        default:
            printf("other type %i\n", ident_type);
            break;
    }
}
