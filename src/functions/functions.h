#ifndef FUNCTIONS
#define FUNCTIONS
#include "../compiler.h"
#include "../parser.h"

typedef enum FuncType {
    FUNCTYPE_None,
    FUNCTYPE_method,
    FUNCTYPE_function,
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
} FuncOpt;

FuncOpt FUNCTION_new();
FuncOpt FUNCTION_parse(Expression *P_expr);
void FUNCTION_write_to_file(Compiler *P_comp, FuncOpt opt );
#endif
