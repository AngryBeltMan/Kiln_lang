#ifndef FUNCTIONS
#define FUNCTIONS
#include "../compiler/compiler.h"
#include "../parser/parser.h"
static int INITED_MAIN = 0;

typedef enum FuncType {
    FUNCTYPE_None,
    FUNCTYPE_method,
    FUNCTYPE_function,
    FUNCTYPE_main
} FuncType;

typedef struct {
    char* module_name;
    char* function;
} FuncMap;

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
    int is_public;
    char* method;
} FuncOpt;

FuncOpt FUNCTION_new();

// DOCS: parse the func expression
FuncOpt FUNCTION_parse(Expression *P_expr);

void FUNCTION_write_to_file(Compiler *P_comp, FuncOpt opt, char* module_name);
#endif
