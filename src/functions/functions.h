#ifndef FUNCTIONS
#define FUNCTIONS
#include "../compiler/compiler.h"
#include "../parser/parser.h"

typedef enum FuncType {
    FUNCTYPE_None,
    FUNCTYPE_method,
    FUNCTYPE_function,
    FUNCTYPE_main
} FuncType;

typedef union FuncArgs {
    struct { char *struct_name; } method;
    struct {} function;
} FuncArgs;

typedef struct FunctionOpt {
    FuncType fn_type;
    FuncArgs fn_args;
    char *return_type;
    char *name;
    char *args;
    int inline_fn;
} FuncOpt;

FuncOpt FUNCTION_new();
FuncOpt FUNCTION_parse(Expression *P_expr);
void FUNCTION_write_to_file(Compiler *P_comp, FuncOpt opt );
#endif
