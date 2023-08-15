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
#include "../contents.c"
#include "../kiln_modules/kiln_module.h"
#include "../kiln_modules/kiln_module.c"
#include <assert.h>
#include <stdio.h>

#define TOKEN_CMP(ident, name, type) \
    if (!strcmp(ident, name)) { \
        return type; \
    }

#define COMPILER_PARSE(ident_type, parse_fn, varopts, OptType) \
    case ident_type: { \
        OptType opts = parse_fn(&P_exprs->exprs[index]); \
        varopts(P_comp, opts); \
        break; \
     }

IdentType get_ident_type(char *ident) {
    printf("%s",ident);
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
    TOKEN_CMP(ident, "bring", IdentType_import_module);
    TOKEN_CMP(ident, "export_name", IdentType_export_module_name);
    TOKEN_CMP(&ident[0], "}", IdentType_break_bracket);
    printf("varname %s\n",ident);
    return IdentType_var_name;
}

void write_print(Compiler *P_comp, Expression *P_expr, char *formatting) {
    COMPILER_add_module(P_comp, MODULE_stdio);
    Contents contents = token_string_parse(P_expr, 3);
    CONTENTS_append_formatted(&P_comp->contents, "printf(\"%s%s", contents.file, formatting);
    free(contents.file);
    contents.file = NULL;
}

// P_module_name is not an array of char* instead it is a mutable pointer to chars
void parse_expression_from_keyword(IdentType ident_type, Compiler *P_comp, Expressions *P_exprs, int index, Hashmap* var_map,Hashmap* P_func_map,  char** P_module_name) {
    switch (ident_type) {
        // println("hello world");
        case IdentType_println: {
            write_print(P_comp, &P_exprs->exprs[index], "\\n\");\n");
            break;
        }
        case IdentType_print:{
            write_print(P_comp, &P_exprs->exprs[index], "\");\n");
            break;
        }
        // let num $int = 10;
        case IdentType_var_init: {
            VarOpts varopts = VAROPTS_expression_parse(&P_exprs->exprs[index], P_comp, var_map);
            VAROPTS_create_var(varopts, P_comp);
            break;
        }
        COMPILER_PARSE(IdentType_if_statement, IFSTATEMENT_parse, IFSTATEMENT_write_to_file, IfStatementOpt)
        COMPILER_PARSE(IdentType_for_loop, FORLOOP_parse, FORLOOP_write_to_file, ForLoopOpt)
        COMPILER_PARSE(IdentType_struct_init, STRUCT_parse_file, STRUCT_write_to_file, StructOpt)
        case IdentType_function: {
            FuncOpt func_opts = FUNCTION_parse(&P_exprs->exprs[index]);
            FUNCTION_write_to_file(P_comp, func_opts, *P_module_name);
            break;
        }
        case IdentType_var_name: {
            if (P_exprs->exprs[index].size == 8) { break; }
            struct CallsType call = CALLSTYPE_parse(&P_exprs->exprs[index], var_map, P_func_map, *P_module_name, 0);
            CALLSTYPE_write_to_file(P_comp, call);
            CALLSTYPE_free(call);
            break;
        }
        case IdentType_return_fn: {
            char* return_val = token_parse_expression_to_end(&P_exprs->exprs[index],1).file;
            CONTENTS_append_formatted(&P_comp->contents, "return %s;\n",return_val);
            free(return_val);
            break;
        }
        case IdentType_export_module_name: {
            assert(P_exprs->exprs[index].size / sizeof(Token) > 1 && "Error: expected module name");
            assert(P_exprs->exprs[index].tokens->token_type == TokenType_Ident && "Error: module name is not of type ident");
            *P_module_name = P_exprs->exprs[index].tokens[1].value;
            break;
        }
        case IdentType_import_module: {
            KilnModule opts = KILNMODULE_parse(&P_exprs->exprs[index], P_func_map, &P_comp->included_files );
            KILNMODULE_write_to_file(P_comp, opts);
            break;
        }
        default: {
            break;
        }
    }
}
